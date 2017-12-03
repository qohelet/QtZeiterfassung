#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QUrl>
#include <QLineEdit>
#include <QMessageBox>
#include <QStandardItem>
#include <QStringBuilder>
#include <QMenu>
#include <QLabel>
#include <QDir>
#include <QTemporaryFile>
#include <QDesktopServices>
#include <QRegularExpression>
#include <QDebug>

#include "zeiterfassungsettings.h"
#include "eventloopwithstatus.h"
#include "dialogs/aboutmedialog.h"
#include "dialogs/buchungdialog.h"
#include "dialogs/kontierungdialog.h"
#include "strips/buchungstrip.h"
#include "strips/kontierungstrip.h"
#include "models/buchungenmodel.h"
#include "models/kontierungenmodel.h"

MainWindow::MainWindow(ZeiterfassungSettings &settings, Zeiterfassung &erfassung, const Zeiterfassung::UserInfo &userInfo, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_settings(settings),
    m_erfassung(erfassung),
    m_userInfo(userInfo),
    m_buchungenModel(new BuchungenModel(erfassung, this)),
    m_kontierungenModel(new KontierungenModel(erfassung, this)),
    m_flag(false)
{
    ui->setupUi(this);

    setWindowTitle(tr("Zeiterfassung - %0 (%1)").arg(m_userInfo.text).arg(m_userInfo.email));

    ui->actionQuit->setShortcut(QKeySequence::Quit);

    connect(ui->actionToday, &QAction::triggered, [=](){ ui->dateEditDate->setDate(QDate::currentDate()); });

    ui->actionRefresh->setShortcut(QKeySequence::Refresh);
    connect(ui->actionRefresh, &QAction::triggered, this, &MainWindow::refresh);

    connect(ui->actionAuswertung, &QAction::triggered, [=](){
        QTemporaryFile file(QDir::temp().absoluteFilePath("auswertungXXXXXX.pdf"));
        file.setAutoRemove(false);
        if(!file.open())
        {
            QMessageBox::warning(this, tr("Could not open auswertung!"), tr("Auswertung could not be written:\n\n%0")
                                 .arg(file.errorString()));
            return;
        }

        file.write(m_auswertung);
        file.close();

        if(!QDesktopServices::openUrl(QUrl::fromLocalFile(file.fileName())))
        {
            QMessageBox::warning(this, tr("Could not open auswertung!"), tr("Could not open default PDF viewer!"));
            return;
        }
    });

    connect(ui->actionAboutMe, &QAction::triggered, [=](){ AboutMeDialog(userInfo, this).exec(); });
    connect(ui->actionAboutQt, &QAction::triggered, [=](){ QMessageBox::aboutQt(this); });

    ui->dateEditDate->setDate(QDate::currentDate());
    connect(ui->dateEditDate, &QDateTimeEdit::dateChanged, this, &MainWindow::refresh);
    refresh();

    connect(ui->pushButtonPrev, &QAbstractButton::pressed, this, &MainWindow::pushButtonPrevPressed);
    connect(ui->pushButtonNext, &QAbstractButton::pressed, this, &MainWindow::pushButtonNextPressed);

    ui->timeEditTime->setTime(timeNormalise(QTime::currentTime()));

    connect(&m_erfassung, &Zeiterfassung::getProjekteFinished,
            this, &MainWindow::getProjekteFinished);
    erfassung.doGetProjekte(userInfo.userId, QDate::currentDate());

    ui->comboBoxProjekt->setMaxVisibleItems(10);

    ui->comboBoxSubprojekt->lineEdit()->setPlaceholderText(tr("Subprojekt"));
    ui->comboBoxWorkpackage->lineEdit()->setPlaceholderText(tr("Workpackage"));
    ui->comboBoxText->lineEdit()->setPlaceholderText(tr("Text"));

    updateComboboxes();

    connect(ui->pushButtonStart, &QAbstractButton::pressed, this, &MainWindow::pushButtonStartPressed);
    connect(ui->pushButtonEnd, &QAbstractButton::pressed, this, &MainWindow::pushButtonEndPressed);

    ui->treeViewBuchungen->setModel(m_buchungenModel);
    ui->treeViewKontierungen->setModel(m_kontierungenModel);

    connect(ui->treeViewBuchungen, &QWidget::customContextMenuRequested,
            this,                  &MainWindow::contextMenuBuchung);
    connect(ui->treeViewKontierungen, &QWidget::customContextMenuRequested,
            this,                     &MainWindow::contextMenuKontierung);

    ui->statusbar->addPermanentWidget(m_auswertungLabel = new QLabel(ui->statusbar));

    updateAuswertung();
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::timeToSeconds(const QTime &time)
{
    return QTime(0, 0).secsTo(time);
}

QTime MainWindow::timeBetween(const QTime &l, const QTime &r)
{
    Q_ASSERT(l <= r);
    return QTime(0, 0).addSecs(l.secsTo(r));
}

QTime MainWindow::timeAdd(const QTime &l, const QTime &r)
{
    Q_ASSERT(timeToSeconds(l) + timeToSeconds(r) < 86400);
    return l.addSecs(QTime(0, 0).secsTo(r));
}

QTime MainWindow::timeNormalise(const QTime &time)
{
    return QTime(time.hour(), time.minute());
}

void MainWindow::refresh()
{
    ui->actionToday->setEnabled(false);
    ui->actionRefresh->setEnabled(false);
    ui->dateEditDate->setReadOnly(true);
    ui->pushButtonPrev->setEnabled(false);
    ui->pushButtonNext->setEnabled(false);
    ui->timeEditTime->setEnabled(false);
    ui->comboBoxProjekt->setEnabled(false);
    ui->comboBoxSubprojekt->setEnabled(false);
    ui->comboBoxWorkpackage->setEnabled(false);
    ui->comboBoxText->setEnabled(false);
    ui->pushButtonStart->setEnabled(false);
    ui->pushButtonEnd->setEnabled(false);
    ui->treeViewBuchungen->setEnabled(false);
    ui->treeViewKontierungen->setEnabled(false);

    auto waitForBuchugen = m_buchungenModel->refresh(m_userInfo.userId, ui->dateEditDate->date(), ui->dateEditDate->date());
    if(waitForBuchugen)
    {
        connect(m_buchungenModel, &BuchungenModel::refreshFinished,
                this,             &MainWindow::refreshBuchungenFinished);
    }

    auto waitForKontierungen = m_kontierungenModel->refresh(m_userInfo.userId, ui->dateEditDate->date(), ui->dateEditDate->date());
    if(waitForKontierungen)
    {
        connect(m_kontierungenModel, &KontierungenModel::refreshFinished,
                this,                &MainWindow::refreshKontierungenFinished);
    }

    if(!waitForBuchugen || !waitForKontierungen)
        QMessageBox::warning(this, tr("Unknown error occured."), tr("An unknown error occured."));

    if(waitForBuchugen || waitForKontierungen)
        m_flag = waitForBuchugen == waitForKontierungen;
    else
    {
        ui->actionToday->setEnabled(true);
        ui->actionRefresh->setEnabled(true);
        ui->dateEditDate->setReadOnly(false);
        ui->pushButtonPrev->setEnabled(true);
        ui->pushButtonNext->setEnabled(true);
    }

    clearStrips();
}

void MainWindow::getProjekteFinished(bool success, const QString &message, const QVector<Zeiterfassung::Projekt> &projekte)
{
    disconnect(&m_erfassung, &Zeiterfassung::getProjekteFinished,
               this, &MainWindow::getProjekteFinished);

    if(!success)
    {
        QMessageBox::warning(this, tr("Could not load Buchungen!"), tr("Could not load Buchungen:\n\n%0").arg(message));
        return;
    }

    m_projekte.clear();

    for(const auto &projekt : projekte)
        m_projekte.insert(projekt.value, projekt.label);

    updateComboboxes();
}

void MainWindow::getAuswertungFinished(bool success, const QString &message, const QByteArray &content)
{
    disconnect(&m_erfassung, &Zeiterfassung::getAuswertungFinished,
               this,         &MainWindow::getAuswertungFinished);

    if(!success)
    {
        QMessageBox::warning(this, tr("Could not load Auswertung!"), tr("Could not load Auswertung:\n\n%0").arg(message));
        return;
    }

    ui->actionAuswertung->setEnabled(true);
    m_auswertung = content;

    QString normalArbeitszeit = QStringLiteral("???");
    QString urlaubsAnspruch = QStringLiteral("???");
    QString gleitzeit = QStringLiteral("???");

    {
        static QRegularExpression regex("Normalarbeitsze +([0-9]+\\:[0-9]+\\-?) +([0-9]+\\:[0-9]+\\-?)");
        auto match = regex.match(content);
        if(match.hasMatch())
        {
            normalArbeitszeit = match.captured(2);
            qDebug() << "Normalarbeitszeit" << match.captured(1) << match.captured(2);
        }
        else
            qWarning() << "Normalarbeitszeit not found";
    }
    {
        static QRegularExpression regex("Urlaubsanspruch +([0-9]+\\.[0-9]+\\-?) +([0-9]+\\.[0-9]+\\-?)");
        auto match = regex.match(content);
        if(match.hasMatch())
        {
            urlaubsAnspruch = match.captured(2);
            qDebug() << "Urlaubsanspruch" << match.captured(1) << match.captured(2);
        }
        else
            qWarning() << "Urlaubsanspruch not found";
    }
    {
        static QRegularExpression regex("Gleitzeit +([0-9]+\\:[0-9]+\\-?) +([0-9]+\\:[0-9]+\\-?)");
        auto match = regex.match(content);
        if(match.hasMatch())
        {
            gleitzeit = match.captured(2);
            qDebug() << "Gleitzeit" << match.captured(1) << match.captured(2);
        }
        else
            qWarning() << "Gleitzeit not found";
    }

    m_auswertungLabel->setText(tr("Urlaubsanspruch: %0 Gleitzeit: %1").arg(urlaubsAnspruch).arg(gleitzeit));
}

void MainWindow::refreshBuchungenFinished(bool success, const QString &message)
{
    disconnect(m_buchungenModel, &BuchungenModel::refreshFinished,
               this,             &MainWindow::refreshBuchungenFinished);

    if(success)
        ui->treeViewBuchungen->setEnabled(true);

    if(m_flag)
        m_flag = false;
    else
        validateEntries();

    if(!success)
        QMessageBox::warning(Q_NULLPTR, tr("Could not refresh Buchungen!"), tr("Could not refresh Buchungen:\n\n%0").arg(message));
}

void MainWindow::refreshKontierungenFinished(bool success, const QString &message)
{
    disconnect(m_kontierungenModel, &KontierungenModel::refreshFinished,
               this,                &MainWindow::refreshKontierungenFinished);

    if(success)
        ui->treeViewKontierungen->setEnabled(true);

    if(m_flag)
        m_flag = false;
    else
        validateEntries();

    if(!success)
        QMessageBox::warning(Q_NULLPTR, tr("Could not refresh Kontierungen!"), tr("Could not refresh Kontierungen:\n\n%0").arg(message));
}

void MainWindow::contextMenuBuchung(const QPoint &pos)
{
    auto index = ui->treeViewBuchungen->indexAt(pos);

    if(index.isValid())
    {
        auto buchung = m_buchungenModel->getBuchung(index);

        QMenu menu;
        auto editAction = menu.addAction(tr("Edit"));
        auto deleteAction = menu.addAction(tr("Delete"));
        auto selectedAction = menu.exec(ui->treeViewBuchungen->viewport()->mapToGlobal(pos));
        if(selectedAction == editAction)
        {
            BuchungDialog dialog(this);
            dialog.setTime(buchung.time);
            dialog.setTimespan(buchung.timespan);
            dialog.setType(buchung.type);
            dialog.setText(buchung.text);
            again1:
            if(dialog.exec() == QDialog::Accepted)
            {
                EventLoopWithStatus eventLoop;
                connect(&m_erfassung, &Zeiterfassung::updateBuchungFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

                m_erfassung.doUpdateBuchung(buchung.id, m_userInfo.userId, ui->dateEditDate->date(),
                                            dialog.getTime(), dialog.getTimespan(),
                                            dialog.getType(), dialog.getText());
                eventLoop.exec();

                if(eventLoop.success())
                {
                    ui->actionToday->setEnabled(false);
                    ui->actionRefresh->setEnabled(false);
                    ui->dateEditDate->setReadOnly(true);
                    ui->pushButtonPrev->setEnabled(false);
                    ui->pushButtonNext->setEnabled(false);
                    ui->timeEditTime->setEnabled(false);
                    ui->comboBoxProjekt->setEnabled(false);
                    ui->comboBoxSubprojekt->setEnabled(false);
                    ui->comboBoxWorkpackage->setEnabled(false);
                    ui->comboBoxText->setEnabled(false);
                    ui->pushButtonStart->setEnabled(false);
                    ui->pushButtonEnd->setEnabled(false);
                    ui->treeViewBuchungen->setEnabled(false);

                    clearStrips();

                    if(m_buchungenModel->refresh(m_userInfo.userId, ui->dateEditDate->date(), ui->dateEditDate->date()))
                    {
                        connect(m_buchungenModel, &BuchungenModel::refreshFinished,
                                this,             &MainWindow::refreshBuchungenFinished);
                        m_flag = false;
                    }
                    else
                    {
                        ui->actionToday->setEnabled(true);
                        ui->actionRefresh->setEnabled(true);
                        ui->dateEditDate->setReadOnly(false);
                        ui->pushButtonPrev->setEnabled(true);
                        ui->pushButtonNext->setEnabled(true);
                    }
                }
                else
                {
                    QMessageBox::warning(this, tr("Could not update Buchung!"), tr("Could not update Buchung:\n\n%0").arg(eventLoop.message()));
                    goto again1;
                }
            }
        }
        else if(selectedAction == deleteAction)
        {
            QMessageBox msgBox;
            msgBox.setText("Do you really want to delete the Buchung?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            if(msgBox.exec() == QMessageBox::Yes)
            {
                EventLoopWithStatus eventLoop;
                connect(&m_erfassung, &Zeiterfassung::deleteBuchungFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

                m_erfassung.doDeleteBuchung(buchung.id);
                eventLoop.exec();

                if(eventLoop.success())
                {
                    ui->actionToday->setEnabled(false);
                    ui->actionRefresh->setEnabled(false);
                    ui->dateEditDate->setReadOnly(true);
                    ui->pushButtonPrev->setEnabled(false);
                    ui->pushButtonNext->setEnabled(false);
                    ui->timeEditTime->setEnabled(false);
                    ui->comboBoxProjekt->setEnabled(false);
                    ui->comboBoxSubprojekt->setEnabled(false);
                    ui->comboBoxWorkpackage->setEnabled(false);
                    ui->comboBoxText->setEnabled(false);
                    ui->pushButtonStart->setEnabled(false);
                    ui->pushButtonEnd->setEnabled(false);
                    ui->treeViewBuchungen->setEnabled(false);

                    clearStrips();

                    if(m_buchungenModel->refresh(m_userInfo.userId, ui->dateEditDate->date(), ui->dateEditDate->date()))
                    {
                        connect(m_buchungenModel, &BuchungenModel::refreshFinished,
                                this,             &MainWindow::refreshBuchungenFinished);
                        m_flag = false;
                    }
                    else
                    {
                        ui->actionToday->setEnabled(true);
                        ui->actionRefresh->setEnabled(true);
                        ui->dateEditDate->setReadOnly(false);
                        ui->pushButtonPrev->setEnabled(true);
                        ui->pushButtonNext->setEnabled(true);
                    }
                }
                else
                    QMessageBox::warning(this, tr("Could not delete Buchung!"), tr("Could not delete Buchung:\n\n%0").arg(eventLoop.message()));
            }
        }
    }
    else
    {
        QMenu menu;
        auto createAction = menu.addAction(tr("Create"));
        auto selectedAction = menu.exec(ui->treeViewBuchungen->viewport()->mapToGlobal(pos));
        if(selectedAction == createAction)
        {
            BuchungDialog dialog(this);
            dialog.setTime(timeNormalise(QTime::currentTime()));
            again2:
            if(dialog.exec() == QDialog::Accepted)
            {
                EventLoopWithStatus eventLoop;
                connect(&m_erfassung, &Zeiterfassung::createBuchungFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

                m_erfassung.doCreateBuchung(m_userInfo.userId, ui->dateEditDate->date(),
                                            dialog.getTime(), dialog.getTimespan(),
                                            dialog.getType(), dialog.getText());
                eventLoop.exec();

                if(eventLoop.success())
                {
                    ui->actionToday->setEnabled(false);
                    ui->actionRefresh->setEnabled(false);
                    ui->dateEditDate->setReadOnly(true);
                    ui->pushButtonPrev->setEnabled(false);
                    ui->pushButtonNext->setEnabled(false);
                    ui->timeEditTime->setEnabled(false);
                    ui->comboBoxProjekt->setEnabled(false);
                    ui->comboBoxSubprojekt->setEnabled(false);
                    ui->comboBoxWorkpackage->setEnabled(false);
                    ui->comboBoxText->setEnabled(false);
                    ui->pushButtonStart->setEnabled(false);
                    ui->pushButtonEnd->setEnabled(false);
                    ui->treeViewBuchungen->setEnabled(false);

                    clearStrips();

                    if(m_buchungenModel->refresh(m_userInfo.userId, ui->dateEditDate->date(), ui->dateEditDate->date()))
                    {
                        connect(m_buchungenModel, &BuchungenModel::refreshFinished,
                                this,             &MainWindow::refreshBuchungenFinished);
                        m_flag = false;
                    }
                    else
                    {
                        ui->actionToday->setEnabled(true);
                        ui->actionRefresh->setEnabled(true);
                        ui->dateEditDate->setReadOnly(false);
                        ui->pushButtonPrev->setEnabled(true);
                        ui->pushButtonNext->setEnabled(true);
                    }
                }
                else
                {
                    QMessageBox::warning(this, tr("Could not create Buchung!"), tr("Could not create Buchung:\n\n%0").arg(eventLoop.message()));
                    goto again2;
                }
            }
        }
    }
}

void MainWindow::contextMenuKontierung(const QPoint &pos)
{
    auto index = ui->treeViewKontierungen->indexAt(pos);

    if(index.isValid())
    {
        auto kontierung = m_kontierungenModel->getKontierung(index);

        QMenu menu;
        auto editAction = menu.addAction(tr("Edit"));
        auto deleteAction = menu.addAction(tr("Delete"));
        auto selectedAction = menu.exec(ui->treeViewKontierungen->viewport()->mapToGlobal(pos));
        if(selectedAction == editAction)
        {
            KontierungDialog dialog(m_projekte, m_settings, this);
            dialog.setTime(kontierung.time);
            dialog.setTimespan(kontierung.timespan);
            dialog.setProjekt(kontierung.projekt);
            dialog.setSubprojekt(kontierung.subprojekt);
            dialog.setWorkpackage(kontierung.workpackage);
            dialog.setText(kontierung.text);
            again1:
            if(dialog.exec() == QDialog::Accepted)
            {
                EventLoopWithStatus eventLoop;
                connect(&m_erfassung, &Zeiterfassung::updateKontierungFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

                m_erfassung.doUpdateKontierung(kontierung.id, m_userInfo.userId, ui->dateEditDate->date(),
                                               dialog.getTime(), dialog.getTimespan(),
                                               dialog.getProjekt(), dialog.getSubprojekt(),
                                               dialog.getWorkpackage(), dialog.getText());
                eventLoop.exec();

                if(eventLoop.success())
                {
                    ui->actionToday->setEnabled(false);
                    ui->actionRefresh->setEnabled(false);
                    ui->dateEditDate->setReadOnly(true);
                    ui->pushButtonPrev->setEnabled(false);
                    ui->pushButtonNext->setEnabled(false);
                    ui->timeEditTime->setEnabled(false);
                    ui->comboBoxProjekt->setEnabled(false);
                    ui->comboBoxSubprojekt->setEnabled(false);
                    ui->comboBoxWorkpackage->setEnabled(false);
                    ui->comboBoxText->setEnabled(false);
                    ui->pushButtonStart->setEnabled(false);
                    ui->pushButtonEnd->setEnabled(false);
                    ui->treeViewKontierungen->setEnabled(false);

                    m_settings.prependProjekt(dialog.getProjekt());
                    m_settings.prependSubprojekt(dialog.getSubprojekt());
                    m_settings.prependWorkpackage(dialog.getWorkpackage());
                    m_settings.prependText(dialog.getText());

                    clearStrips();

                    if(m_kontierungenModel->refresh(m_userInfo.userId, ui->dateEditDate->date(), ui->dateEditDate->date()))
                    {
                        connect(m_kontierungenModel, &KontierungenModel::refreshFinished,
                                this,                &MainWindow::refreshKontierungenFinished);
                        m_flag = false;
                    }
                    else
                    {
                        ui->actionToday->setEnabled(true);
                        ui->actionRefresh->setEnabled(true);
                        ui->dateEditDate->setReadOnly(false);
                        ui->pushButtonPrev->setEnabled(true);
                        ui->pushButtonNext->setEnabled(true);
                    }
                }
                else
                {
                    QMessageBox::warning(this, tr("Could not update Kontierung!"), tr("Could not update Kontierung:\n\n%0").arg(eventLoop.message()));
                    goto again1;
                }
            }
        }
        else if(selectedAction == deleteAction)
        {
            QMessageBox msgBox;
            msgBox.setText("Do you really want to delete the Kontierung?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            if(msgBox.exec() == QMessageBox::Yes)
            {
                EventLoopWithStatus eventLoop;
                connect(&m_erfassung, &Zeiterfassung::deleteKontierungFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

                m_erfassung.doDeleteKontierung(kontierung.id);
                eventLoop.exec();

                if(eventLoop.success())
                {
                    ui->actionToday->setEnabled(false);
                    ui->actionRefresh->setEnabled(false);
                    ui->dateEditDate->setReadOnly(true);
                    ui->pushButtonPrev->setEnabled(false);
                    ui->pushButtonNext->setEnabled(false);
                    ui->timeEditTime->setEnabled(false);
                    ui->comboBoxProjekt->setEnabled(false);
                    ui->comboBoxSubprojekt->setEnabled(false);
                    ui->comboBoxWorkpackage->setEnabled(false);
                    ui->comboBoxText->setEnabled(false);
                    ui->pushButtonStart->setEnabled(false);
                    ui->pushButtonEnd->setEnabled(false);
                    ui->treeViewKontierungen->setEnabled(false);

                    clearStrips();

                    if(m_kontierungenModel->refresh(m_userInfo.userId, ui->dateEditDate->date(), ui->dateEditDate->date()))
                    {
                        connect(m_kontierungenModel, &KontierungenModel::refreshFinished,
                                this,                &MainWindow::refreshKontierungenFinished);
                        m_flag = false;
                    }
                    else
                    {
                        ui->actionToday->setEnabled(true);
                        ui->actionRefresh->setEnabled(true);
                        ui->dateEditDate->setReadOnly(false);
                        ui->pushButtonPrev->setEnabled(true);
                        ui->pushButtonNext->setEnabled(true);
                    }
                }
                else
                    QMessageBox::warning(this, tr("Could not delete Kontierung!"), tr("Could not delete Kontierung:\n\n%0").arg(eventLoop.message()));
            }
        }
    }
    else
    {
        QMenu menu;
        auto createAction = menu.addAction(tr("Create"));
        auto selectedAction = menu.exec(ui->treeViewKontierungen->viewport()->mapToGlobal(pos));
        if(selectedAction == createAction)
        {
            KontierungDialog dialog(m_projekte, m_settings, this);
            again2:
            if(dialog.exec() == QDialog::Accepted)
            {
                EventLoopWithStatus eventLoop;
                connect(&m_erfassung, &Zeiterfassung::createKontierungFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

                m_erfassung.doCreateKontierung(m_userInfo.userId, ui->dateEditDate->date(),
                                               dialog.getTime(), dialog.getTimespan(),
                                               dialog.getProjekt(), dialog.getSubprojekt(),
                                               dialog.getWorkpackage(), dialog.getText());
                eventLoop.exec();

                if(eventLoop.success())
                {
                    ui->actionToday->setEnabled(false);
                    ui->actionRefresh->setEnabled(false);
                    ui->dateEditDate->setReadOnly(true);
                    ui->pushButtonPrev->setEnabled(false);
                    ui->pushButtonNext->setEnabled(false);
                    ui->timeEditTime->setEnabled(false);
                    ui->comboBoxProjekt->setEnabled(false);
                    ui->comboBoxSubprojekt->setEnabled(false);
                    ui->comboBoxWorkpackage->setEnabled(false);
                    ui->comboBoxText->setEnabled(false);
                    ui->pushButtonStart->setEnabled(false);
                    ui->pushButtonEnd->setEnabled(false);
                    ui->treeViewKontierungen->setEnabled(false);

                    m_settings.prependProjekt(dialog.getProjekt());
                    m_settings.prependSubprojekt(dialog.getSubprojekt());
                    m_settings.prependWorkpackage(dialog.getWorkpackage());
                    m_settings.prependText(dialog.getText());

                    clearStrips();

                    if(m_kontierungenModel->refresh(m_userInfo.userId, ui->dateEditDate->date(), ui->dateEditDate->date()))
                    {
                        connect(m_kontierungenModel, &KontierungenModel::refreshFinished,
                                this,                &MainWindow::refreshKontierungenFinished);
                        m_flag = false;
                    }
                    else
                    {
                        ui->actionToday->setEnabled(true);
                        ui->actionRefresh->setEnabled(true);
                        ui->dateEditDate->setReadOnly(false);
                        ui->pushButtonPrev->setEnabled(true);
                        ui->pushButtonNext->setEnabled(true);
                    }
                }
                else
                {
                    QMessageBox::warning(this, tr("Could not create Kontierung!"), tr("Could not create Kontierung:\n\n%0").arg(eventLoop.message()));
                    goto again2;
                }
            }
        }
    }
}

void MainWindow::pushButtonPrevPressed()
{
    ui->dateEditDate->setDate(ui->dateEditDate->date().addDays(-1));
}

void MainWindow::pushButtonNextPressed()
{
    ui->dateEditDate->setDate(ui->dateEditDate->date().addDays(1));
}

void MainWindow::pushButtonStartPressed()
{
    if(m_buchungenModel->rbegin() == m_buchungenModel->rend() ||
       m_buchungenModel->rbegin()->type == QStringLiteral("G"))
    {
        EventLoopWithStatus eventLoop;
        connect(&m_erfassung, &Zeiterfassung::createBuchungFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

        m_erfassung.doCreateBuchung(m_userInfo.userId, ui->dateEditDate->date(),
                                    timeNormalise(ui->timeEditTime->time()), QTime(0, 0),
                                    QStringLiteral("K"), QStringLiteral(""));
        eventLoop.exec();

        if(!eventLoop.success())
        {
            QMessageBox::warning(this, tr("Could not create Buchung!"), tr("Could not create Buchung:\n\n%0").arg(eventLoop.message()));
            refresh();
            return;
        }
    }

    if(m_kontierungenModel->rbegin() != m_kontierungenModel->rend())
    {
        auto kontierung = *m_kontierungenModel->rbegin();
        if(kontierung.timespan == QTime(0, 0))
        {
            EventLoopWithStatus eventLoop;
            connect(&m_erfassung, &Zeiterfassung::updateKontierungFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

            auto timespan = timeBetween(m_lastKontierungStart, ui->timeEditTime->time());

            m_erfassung.doUpdateKontierung(kontierung.id, m_userInfo.userId, kontierung.date,
                                           kontierung.time, timespan,
                                           kontierung.projekt, kontierung.subprojekt,
                                           kontierung.workpackage, kontierung.text);
            eventLoop.exec();

            if(eventLoop.success())
                m_kontierungTime = timeAdd(m_kontierungTime, timespan);
            else
            {
                QMessageBox::warning(this, tr("Could not update Kontierung!"), tr("Could not update Kontierung:\n\n%0").arg(eventLoop.message()));
                refresh();
                return;
            }
        }
    }

    EventLoopWithStatus eventLoop;
    connect(&m_erfassung, &Zeiterfassung::createKontierungFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

    m_erfassung.doCreateKontierung(m_userInfo.userId, ui->dateEditDate->date(),
                                   m_kontierungTime, QTime(0, 0),
                                   ui->comboBoxProjekt->currentData().toString(), ui->comboBoxSubprojekt->currentText(),
                                   ui->comboBoxWorkpackage->currentText(), ui->comboBoxText->currentText());
    eventLoop.exec();

    if(!eventLoop.success())
    {
        QMessageBox::warning(this, tr("Could not create Kontierung!"), tr("Could not create Kontierung:\n\n%0").arg(eventLoop.message()));
        refresh();
        return;
    }

    m_settings.prependProjekt(ui->comboBoxProjekt->currentData().toString());
    m_settings.prependSubprojekt(ui->comboBoxSubprojekt->currentText());
    m_settings.prependWorkpackage(ui->comboBoxWorkpackage->currentText());
    m_settings.prependText(ui->comboBoxText->currentText());

    updateComboboxes();

    refresh();
}

void MainWindow::pushButtonEndPressed()
{
    {
        EventLoopWithStatus eventLoop;
        connect(&m_erfassung, &Zeiterfassung::createBuchungFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

        m_erfassung.doCreateBuchung(m_userInfo.userId, ui->dateEditDate->date(),
                                    timeNormalise(ui->timeEditTime->time()), QTime(0, 0),
                                    QStringLiteral("G"), QStringLiteral(""));
        eventLoop.exec();

        if(!eventLoop.success())
        {
            QMessageBox::warning(this, tr("Could not create Buchung!"), tr("Could not create Buchung:\n\n%0").arg(eventLoop.message()));
            refresh();
            return;
        }
    }

    {
        auto kontierung = *m_kontierungenModel->rbegin();
        Q_ASSERT(kontierung.timespan == QTime(0, 0));

        EventLoopWithStatus eventLoop;
        connect(&m_erfassung, &Zeiterfassung::updateKontierungFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

        auto timespan = timeBetween(m_lastKontierungStart, ui->timeEditTime->time());

        m_erfassung.doUpdateKontierung(kontierung.id, m_userInfo.userId, kontierung.date,
                                       kontierung.time, timespan,
                                       kontierung.projekt, kontierung.subprojekt,
                                       kontierung.workpackage, kontierung.text);
        eventLoop.exec();

        if(eventLoop.success())
            m_kontierungTime = timeAdd(m_kontierungTime, timespan);
        else
        {
            QMessageBox::warning(this, tr("Could not update Kontierung!"), tr("Could not update Kontierung:\n\n%0").arg(eventLoop.message()));
            refresh();
            return;
        }
    }

    refresh();
}

void MainWindow::validateEntries()
{
    ui->timeEditTime->setMinimumTime(QTime(0, 0));
    ui->actionToday->setEnabled(true);
    ui->actionRefresh->setEnabled(true);
    ui->dateEditDate->setReadOnly(false);
    ui->pushButtonPrev->setEnabled(true);
    ui->pushButtonNext->setEnabled(true);
    ui->pushButtonStart->setText(tr("Start"));

    if(!ui->treeViewBuchungen->isEnabled())
        return;

    if(!ui->treeViewKontierungen->isEnabled())
        return;

    auto buchungenIter = m_buchungenModel->constBegin();
    auto kontierungenIter = m_kontierungenModel->constBegin();

    m_kontierungTime = QTime(0, 0);
    auto buchungTimespan = QTime(0, 0);

    QString errorMessage;

    while(true)
    {
        if(buchungenIter == m_buchungenModel->constEnd() &&
           kontierungenIter == m_kontierungenModel->constEnd())
        {
            goto after;
        }

        if(buchungenIter == m_buchungenModel->constEnd())
        {
            errorMessage = tr("Missing Buchung.");
            goto after;
        }

        auto startBuchung = *buchungenIter++;
        if(startBuchung.type != QStringLiteral("K"))
        {
            errorMessage = tr("Expected Buchung for Kommen, instead got type %0\nBuchung ID: %1")
                    .arg(startBuchung.type)
                    .arg(startBuchung.id);
            goto after;
        }

        m_lastKontierungStart = startBuchung.time;
        ui->verticalLayout2->addWidget(new BuchungStrip(startBuchung.id, startBuchung.time, startBuchung.type, m_settings, ui->scrollAreaWidgetContents));

        if(kontierungenIter == m_kontierungenModel->constEnd())
        {
            errorMessage = tr("Missing Kontierung.");
            goto after;
        }

        auto kontierung = *kontierungenIter++;
        if(kontierung.time != m_kontierungTime)
        {
            errorMessage = tr("Expected time %0 but got %1 Kontierung.\nKontierung ID: %2")
                    .arg(m_kontierungTime.toString("HH:mm:ss"))
                    .arg(kontierung.time.toString("HH:mm:ss"))
                    .arg(kontierung.id);
            goto after;
        }

        ui->verticalLayout2->addWidget(new KontierungStrip(kontierung.id, kontierung.timespan, buildProjektString(kontierung.projekt),
                                                           kontierung.subprojekt, kontierung.workpackage, kontierung.text,
                                                           m_settings, ui->scrollAreaWidgetContents));

        if(kontierung.timespan == QTime(0, 0))
        {
            if(buchungenIter != m_buchungenModel->constEnd())
            {
                errorMessage = tr("There is another Buchung after an unfinished Kontierung.\nBuchung ID: %0\nKontierung ID: %1")
                        .arg(buchungenIter->id)
                        .arg(kontierung.id);
                goto after;
            }

            if(kontierungenIter != m_kontierungenModel->constEnd())
            {
                errorMessage = tr("There is another Kontierung after an unfinished Kontierung.\nKontierung ID: %0\nKontierung ID: %1")
                        .arg(kontierungenIter->id)
                        .arg(kontierung.id);
                goto after;
            }

            ui->timeEditTime->setMinimumTime(timeAdd(m_lastKontierungStart, QTime(0, 1)));
            ui->pushButtonStart->setText(tr("Switch"));
            ui->pushButtonEnd->setEnabled(true);
            goto after;
        }
        else
        {
            m_kontierungTime = timeAdd(m_kontierungTime, kontierung.timespan);
            m_lastKontierungStart = timeAdd(m_lastKontierungStart, kontierung.timespan);

            if(buchungenIter == m_buchungenModel->constEnd())
            {
                while(true)
                {
                    if(kontierungenIter == m_kontierungenModel->constEnd())
                    {
                        errorMessage = tr("The last Kontierung is finished without Gehen-Buchung\nKontierung ID: %0")
                                .arg(kontierung.id);
                        goto after;
                    }

                    kontierung = *kontierungenIter++;
                    if(kontierung.time != m_kontierungTime)
                    {
                        errorMessage = tr("Expected time %0 but got %1 Kontierung.\nKontierung ID: %2")
                                .arg(m_kontierungTime.toString("HH:mm:ss"))
                                .arg(kontierung.time.toString("HH:mm:ss"))
                                .arg(kontierung.id);
                        goto after;
                    }

                    ui->verticalLayout2->addWidget(new KontierungStrip(kontierung.id, kontierung.timespan, buildProjektString(kontierung.projekt),
                                                                       kontierung.subprojekt, kontierung.workpackage, kontierung.text,
                                                                       m_settings, ui->scrollAreaWidgetContents));

                    if(kontierung.timespan == QTime(0, 0))
                    {
                        if(kontierungenIter != m_kontierungenModel->constEnd())
                        {
                            errorMessage = tr("There is another Kontierung after an unfinished Kontierung.\n"
                                              "Kontierung ID: %0\nKontierung ID: %1")
                                    .arg(kontierung.id)
                                    .arg(kontierungenIter->id);
                            goto after;
                        }

                        ui->timeEditTime->setMinimumTime(timeAdd(m_lastKontierungStart, QTime(0, 1)));
                        ui->pushButtonStart->setText(tr("Switch"));
                        ui->pushButtonEnd->setEnabled(true);
                        goto after;
                    }
                    else
                    {
                        m_kontierungTime = timeAdd(m_kontierungTime, kontierung.timespan);
                        m_lastKontierungStart = timeAdd(m_lastKontierungStart, kontierung.timespan);
                    }
                }
            }
            else
            {
                auto endBuchung = *buchungenIter++;
                if(endBuchung.type != QStringLiteral("G"))
                {
                    errorMessage = tr("Expected Buchung for Gehen, instead got type %0\nBuchung ID: %1")
                            .arg(endBuchung.type)
                            .arg(endBuchung.id);
                    goto after;
                }

                buchungTimespan = timeAdd(buchungTimespan, timeBetween(startBuchung.time, endBuchung.time));
                ui->timeEditTime->setMinimumTime(timeAdd(endBuchung.time, QTime(0, 1)));

                while(m_kontierungTime < buchungTimespan)
                {
                    if(kontierungenIter == m_kontierungenModel->constEnd())
                    {
                        errorMessage = tr("Missing Kontierung! Time not filled: %0 - %1")
                                .arg(m_kontierungTime.toString("HH:mm:ss"))
                                .arg(buchungTimespan.toString("HH:mm:ss"));
                        goto after;
                    }

                    kontierung = *kontierungenIter++;
                    if(kontierung.time != m_kontierungTime)
                    {
                        errorMessage = tr("Expected time %0 but got %1 Kontierung.\nKontierung ID: %2")
                                .arg(m_kontierungTime.toString("HH:mm:ss"))
                                .arg(kontierung.time.toString("HH:mm:ss"))
                                .arg(kontierung.id);
                        goto after;
                    }

                    ui->verticalLayout2->addWidget(new KontierungStrip(kontierung.id, kontierung.timespan, buildProjektString(kontierung.projekt),
                                                                       kontierung.subprojekt, kontierung.workpackage, kontierung.text,
                                                                       m_settings, ui->scrollAreaWidgetContents));

                    if(kontierung.timespan == QTime(0, 0))
                    {
                        if(buchungenIter != m_buchungenModel->constEnd())
                        {
                            errorMessage = tr("There is another Buchung after an unfinished Kontierung.\n"
                                              "Buchung ID: %0\nKontierung ID: %1")
                                    .arg(buchungenIter->id)
                                    .arg(kontierung.id);
                            goto after;
                        }

                        if(kontierungenIter != m_kontierungenModel->constEnd())
                        {
                            errorMessage = tr("There is another Kontierung after an unfinished Kontierung.\nKontierung ID: %0\nKontierung ID: %1")
                                    .arg(kontierungenIter->id)
                                    .arg(kontierung.id);
                            goto after;
                        }

                        ui->timeEditTime->setMinimumTime(timeAdd(m_lastKontierungStart, QTime(0, 1)));
                        ui->pushButtonStart->setText(tr("Switch"));
                        ui->pushButtonEnd->setEnabled(true);
                        goto after;
                    }
                    else
                    {
                        m_kontierungTime = timeAdd(m_kontierungTime, kontierung.timespan);
                    }
                }

                if(m_kontierungTime > buchungTimespan)
                {
                    auto label = new QLabel(tr("Kontierung timespan too long!"), ui->scrollAreaWidgetContents);
                    ui->verticalLayout2->addWidget(label);
                    label->setMinimumHeight(20);
                    label->setMaximumHeight(20);
                }

                ui->verticalLayout2->addWidget(new BuchungStrip(endBuchung.id, endBuchung.time, endBuchung.type, m_settings, ui->scrollAreaWidgetContents));

                if(m_kontierungTime > buchungTimespan)
                {
                    errorMessage = tr("The Kontierung timespan is longer than the Kommen-Gehen timespan. "
                                      "The tool does not support this yet!\nKontierung: %0\nBuchung: %1")
                            .arg(m_kontierungTime.toString("HH:mm:ss"))
                            .arg(buchungTimespan.toString("HH:mm:ss"));
                    goto after;
                }
                else
                    ui->verticalLayout2->addSpacing(17);
            }
        }
    }

    after:
    if(!errorMessage.isEmpty())
    {
        auto label = new QLabel(tr("Strip rendering aborted due error."), ui->scrollAreaWidgetContents);
        ui->verticalLayout2->addWidget(label);
        label->setMinimumHeight(20);
        label->setMaximumHeight(20);
    }

    ui->verticalLayout2->addStretch(1);

    if(!errorMessage.isEmpty())
    {
        QMessageBox::warning(this, tr("Illegal state!"), tr("Your Buchungen and Kontierungen for this day are in an invalid state:\n\n%0")
                             .arg(errorMessage));
        return;
    }

    ui->timeEditTime->setEnabled(true);
    ui->comboBoxProjekt->setEnabled(true);
    ui->comboBoxSubprojekt->setEnabled(true);
    ui->comboBoxWorkpackage->setEnabled(true);
    ui->comboBoxText->setEnabled(true);
    ui->pushButtonStart->setEnabled(true);
}

void MainWindow::updateComboboxes()
{
    ui->comboBoxProjekt->clear();

    {
        auto preferedProjekte = m_settings.projekte();

        for(const auto &preferedProjekt : preferedProjekte)
        {
            if(!m_projekte.contains(preferedProjekt))
            {
                qWarning() << "cannot find projekt" << preferedProjekt;
                continue;
            }

            ui->comboBoxProjekt->addItem(m_projekte.value(preferedProjekt) % " (" % preferedProjekt % ')', preferedProjekt);
        }

        if(preferedProjekte.count())
            ui->comboBoxProjekt->insertSeparator(ui->comboBoxProjekt->count());

        for(auto iter = m_projekte.constBegin(); iter != m_projekte.constEnd(); iter++)
        {
            if(!preferedProjekte.contains(iter.key()))
                ui->comboBoxProjekt->addItem(iter.value() % " (" % iter.key() % ')', iter.key());
        }
    }

    ui->comboBoxSubprojekt->clear();

    {
        auto subprojekte = m_settings.subprojekte();
        for(const auto &subprojekt : subprojekte)
            ui->comboBoxSubprojekt->addItem(subprojekt);
        if(subprojekte.count())
            ui->comboBoxSubprojekt->setCurrentText(QString());
    }

    ui->comboBoxWorkpackage->clear();

    {
        auto workpackages = m_settings.workpackages();
        for(const auto &workpackage : workpackages)
            ui->comboBoxWorkpackage->addItem(workpackage);
        if(workpackages.count())
            ui->comboBoxWorkpackage->setCurrentText(QString());
    }

    ui->comboBoxText->clear();

    {
        auto texte = m_settings.texte();
        for(const auto &text : texte)
            ui->comboBoxText->addItem(text);
        if(texte.count())
            ui->comboBoxText->setCurrentText(QString());
    }
}

void MainWindow::updateAuswertung()
{
    m_auswertungLabel->setText(tr("Urlaubsanspruch: %0 Gleitzeit: %1")
                               .arg(QStringLiteral("???"))
                               .arg(QStringLiteral("???")));

    ui->actionAuswertung->setEnabled(false);
    m_auswertung.clear();

    if(m_erfassung.doGetAuswertung(m_userInfo.userId, QDate::currentDate()))
        connect(&m_erfassung, &Zeiterfassung::getAuswertungFinished,
                this,         &MainWindow::getAuswertungFinished);
    else
        QMessageBox::warning(this, tr("Unknown error occured."), tr("An unknown error occured."));
}

void MainWindow::clearStrips()
{
    QLayoutItem *item;
    while(item = ui->verticalLayout2->takeAt(0))
    {
        delete item->widget();
        delete item;
    }
}

QString MainWindow::buildProjektString(const QString &projekt)
{
    if(m_projekte.contains(projekt))
        return m_projekte.value(projekt) % " (" % projekt % ")";
    else
        return projekt;
}
