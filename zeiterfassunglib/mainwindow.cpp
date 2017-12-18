#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QUrl>
#include <QLineEdit>
#include <QMessageBox>
#include <QTimer>
#include <QStringBuilder>
#include <QMenu>
#include <QEventLoop>
#include <QLabel>
#include <QDir>
#include <QTemporaryFile>
#include <QDesktopServices>
#include <QRegularExpression>
#include <QDebug>

#include "zeiterfassungapi.h"
#include "timeutils.h"
#include "zeiterfassungsettings.h"
#include "stripfactory.h"
#include "stripswidget.h"
#include "dialogs/aboutmedialog.h"
#include "dialogs/settingsdialog.h"
#include "dialogs/updatedialog.h"
#include "replies/getprojectsreply.h"
#include "replies/getauswertungreply.h"
#include "replies/createbookingreply.h"
#include "replies/createtimeassignmentreply.h"
#include "replies/updatetimeassignmentreply.h"

MainWindow::MainWindow(ZeiterfassungSettings &settings, ZeiterfassungApi &erfassung, const GetUserInfoReply::UserInfo &userInfo,
                       StripFactory &stripFactory, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_settings(settings),
    m_erfassung(erfassung),
    m_userInfo(userInfo),
    m_stripFactory(stripFactory),
    m_getProjectsReply(Q_NULLPTR),
    m_getAuswertungReply(Q_NULLPTR),
    m_currentStripWidget(Q_NULLPTR)
{
    ui->setupUi(this);

    for(quint8 i = 0; i < 7; i++)
    {
        m_stripsWidgets[i] = new StripsWidget(*this, ui->widgetWeek);
        connect(m_stripsWidgets[i], &StripsWidget::refreshingChanged, this, &MainWindow::refreshingChanged);
        ui->layoutWeek->addWidget(m_stripsWidgets[i]);
    }

    setWindowTitle(tr("Zeiterfassung - %0 (%1)").arg(m_userInfo.text).arg(m_userInfo.email));

    ui->actionQuit->setShortcut(QKeySequence::Quit);

    connect(ui->actionToday, &QAction::triggered, [=](){ ui->dateEditDate->setDate(QDate::currentDate()); });

    ui->actionRefresh->setShortcut(QKeySequence::Refresh);
    connect(ui->actionRefresh, &QAction::triggered, this, [=](){ dateChanged(true); });

    connect(ui->actionAuswertung, &QAction::triggered, this, &MainWindow::openAuswertung);

    connect(ui->actionAboutMe, &QAction::triggered, [=](){ AboutMeDialog(userInfo, this).exec(); });
    connect(ui->actionSettings, &QAction::triggered, [=](){ SettingsDialog(m_settings, this).exec(); });

    ui->actionHelp->setShortcut(QKeySequence::HelpContents);

    connect(ui->actionAboutQt, &QAction::triggered, [=](){ QMessageBox::aboutQt(this); });

    ui->dateEditDate->setDate(QDate::currentDate());
    connect(ui->dateEditDate, &QDateTimeEdit::dateChanged, this, [=](){ dateChanged(false); });

    connect(ui->pushButtonPrev, &QAbstractButton::pressed, [=](){ ui->dateEditDate->setDate(ui->dateEditDate->date().addDays(-1)); });
    connect(ui->pushButtonNext, &QAbstractButton::pressed, [=](){ ui->dateEditDate->setDate(ui->dateEditDate->date().addDays(1)); });

    ui->timeEditTime->setTime(timeNormalise(QTime::currentTime()));

    connect(ui->pushButtonNow, &QAbstractButton::pressed, [=](){ ui->timeEditTime->setTime(timeNormalise(QTime::currentTime())); });

    m_getProjectsReply = erfassung.doGetProjects(userInfo.userId, QDate::currentDate());
    connect(m_getProjectsReply.get(), &ZeiterfassungReply::finished, this, &MainWindow::getProjectsFinished);

    ui->comboBoxSubproject->lineEdit()->setPlaceholderText(tr("Subproject"));
    ui->comboBoxWorkpackage->lineEdit()->setPlaceholderText(tr("Workpackage"));
    ui->comboBoxText->lineEdit()->setPlaceholderText(tr("Text"));

    updateComboboxes();

    connect(ui->pushButtonStart, &QAbstractButton::pressed, this, &MainWindow::pushButtonStartPressed);
    connect(ui->pushButtonEnd, &QAbstractButton::pressed, this, &MainWindow::pushButtonEndPressed);

    ui->statusbar->addPermanentWidget(m_balanceLabel = new QLabel(ui->statusbar));
    m_balanceLabel->setFrameShape(QFrame::Panel);
    m_balanceLabel->setFrameShadow(QFrame::Sunken);
    ui->statusbar->addPermanentWidget(m_holidaysLabel = new QLabel(ui->statusbar));
    m_holidaysLabel->setFrameShape(QFrame::Panel);
    m_holidaysLabel->setFrameShadow(QFrame::Sunken);

    dateChanged();

    if(settings.lastUpdateCheck().isNull() || settings.lastUpdateCheck() < QDate::currentDate())
        new UpdateDialog(settings, erfassung.manager(), this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QMenu *MainWindow::menuFile() const
{
    return ui->menuFile;
}

QMenu *MainWindow::menuView() const
{
    return ui->menuView;
}

QMenu *MainWindow::menuTools() const
{
    return ui->menuTools;
}

QMenu *MainWindow::menuAbout() const
{
    return ui->menuAbout;
}

ZeiterfassungSettings &MainWindow::settings() const
{
    return m_settings;
}

ZeiterfassungApi &MainWindow::erfassung() const
{
    return m_erfassung;
}

const GetUserInfoReply::UserInfo &MainWindow::userInfo() const
{
    return m_userInfo;
}

StripFactory &MainWindow::stripFactory() const
{
    return m_stripFactory;
}

const QMap<QString, QString> &MainWindow::projects() const
{
    return m_projects;
}

const std::array<StripsWidget*, 7> &MainWindow::stripsWidgets() const
{
    return m_stripsWidgets;
}

void MainWindow::getProjectsFinished()
{
    if(m_getProjectsReply->success())
    {
        m_projects.clear();

        for(const auto &project : m_getProjectsReply->projects())
            m_projects.insert(project.value, project.label);

        updateComboboxes();
    }
    else
        QMessageBox::warning(this, tr("Could not load bookings!"),
                             tr("Could not load bookings!") % "\n\n" % m_getProjectsReply->message());

    m_getProjectsReply = Q_NULLPTR;
}

void MainWindow::getAuswertungFinished()
{
    if(std::none_of(std::begin(m_stripsWidgets), std::end(m_stripsWidgets), [](StripsWidget *stripsWidget){
        return stripsWidget->refreshing();
    }))
    {
        ui->actionToday->setEnabled(true);
        ui->actionRefresh->setEnabled(true);
        ui->dateEditDate->setReadOnly(false);
        ui->pushButtonPrev->setEnabled(true);
        ui->pushButtonNext->setEnabled(true);
    }

    if(!m_getAuswertungReply->success())
    {
        m_auswertungDate = QDate();
        QMessageBox::warning(this, tr("Could not load Auswertung!"), tr("Could not load Auswertung!") % "\n\n" % m_getAuswertungReply->message());
        m_getAuswertungReply = Q_NULLPTR;
        return;
    }

    ui->actionAuswertung->setEnabled(true);
    m_auswertung = m_getAuswertungReply->auswertung();

    auto urlaubsAnspruch = tr("n/a");

    {
        static QRegularExpression regex(QStringLiteral("Urlaubsanspruch +([0-9]+\\.[0-9]+\\-?) +([0-9]+\\.[0-9]+\\-?)"));
        auto match = regex.match(m_auswertung);
        if(match.hasMatch())
            urlaubsAnspruch = match.captured(2);
        else
            qWarning() << "Urlaubsanspruch not found";
    }

    auto gleitzeit = tr("n/a");

    {
        static QRegularExpression regex(QStringLiteral("Gleitzeit +([0-9]+\\:[0-9]+\\-?) +([0-9]+\\:[0-9]+\\-?)"));
        auto match = regex.match(m_auswertung);
        if(match.hasMatch())
        {
            gleitzeit = match.captured(2);
            if(gleitzeit.endsWith(QChar('-')))
            {
                gleitzeit.chop(1);
                gleitzeit = QChar('-') % gleitzeit;
            }
            gleitzeit = tr("%0h").arg(gleitzeit);
        }
        else
            qWarning() << "Gleitzeit not found";
    }

    m_balanceLabel->setText(tr("%0: %1").arg(tr("Balance")).arg(gleitzeit));
    m_holidaysLabel->setText(tr("%0: %1").arg(tr("Holidays")).arg(urlaubsAnspruch));

    m_getAuswertungReply = Q_NULLPTR;
}

void MainWindow::pushButtonStartPressed()
{
    auto bookingsChanged = false;

    if(m_currentStripWidget->bookings().rbegin() == m_currentStripWidget->bookings().rend() ||
       m_currentStripWidget->bookings().rbegin()->type == QStringLiteral("G"))
    {
        auto reply = m_erfassung.doCreateBooking(m_userInfo.userId, ui->dateEditDate->date(),
                                                 timeNormalise(ui->timeEditTime->time()), QTime(0, 0),
                                                 QStringLiteral("K"), QStringLiteral(""));

        {
            QEventLoop eventLoop;
            connect(reply.get(), &ZeiterfassungReply::finished, &eventLoop, &QEventLoop::quit);
            eventLoop.exec();
        }

        if(!reply->success())
        {
            QMessageBox::warning(this, tr("Could not create booking!"), tr("Could not create booking!") % "\n\n" % reply->message());
            m_currentStripWidget->refresh();
            return;
        }

        bookingsChanged = true;
    }

    auto timeAssignmentTime = m_currentStripWidget->timeAssignmentTime();

    if(m_currentStripWidget->timeAssignments().rbegin() != m_currentStripWidget->timeAssignments().rend())
    {
        auto timeAssignment = *m_currentStripWidget->timeAssignments().rbegin();
        if(timeAssignment.timespan == QTime(0, 0))
        {
            auto timespan = timeBetween(m_currentStripWidget->lastTimeAssignmentStart(), ui->timeEditTime->time());

            auto reply = m_erfassung.doUpdateTimeAssignment(timeAssignment.id, m_userInfo.userId, timeAssignment.date,
                                                            timeAssignment.time, timespan,
                                                            timeAssignment.project, timeAssignment.subproject,
                                                            timeAssignment.workpackage, timeAssignment.text);

            {
                QEventLoop eventLoop;
                connect(reply.get(), &ZeiterfassungReply::finished, &eventLoop, &QEventLoop::quit);
                eventLoop.exec();
            }

            if(reply->success())
                timeAssignmentTime = timeAdd(timeAssignmentTime, timespan);
            else
            {
                QMessageBox::warning(this, tr("Could not edit time assignment!"), tr("Could not edit time assignment!") % "\n\n" % reply->message());
                m_currentStripWidget->refresh();
                return;
            }
        }
    }

    {
        auto reply = m_erfassung.doCreateTimeAssignment(m_userInfo.userId, ui->dateEditDate->date(),
                                                        timeAssignmentTime, QTime(0, 0),
                                                        ui->comboBoxProject->currentData().toString(), ui->comboBoxSubproject->currentText(),
                                                        ui->comboBoxWorkpackage->currentText(), ui->comboBoxText->currentText());

        {
            QEventLoop eventLoop;
            connect(reply.get(), &ZeiterfassungReply::finished, &eventLoop, &QEventLoop::quit);
            eventLoop.exec();
        }

        if(!reply->success())
        {
            QMessageBox::warning(this, tr("Could not create time assignment!"), tr("Could not create time assignment!") % "\n\n" % reply->message());
            m_currentStripWidget->refresh();
            return;
        }
    }

    m_settings.prependProject(ui->comboBoxProject->currentData().toString());
    m_settings.prependSubproject(ui->comboBoxSubproject->currentText());
    m_settings.prependWorkpackage(ui->comboBoxWorkpackage->currentText());
    m_settings.prependText(ui->comboBoxText->currentText());

    updateComboboxes();

    if(bookingsChanged)
    {
        m_currentStripWidget->refresh();

        refreshAuswertung();
    }
    else
        m_currentStripWidget->refreshTimeAssignments();

    ui->actionToday->setEnabled(false);
    ui->actionRefresh->setEnabled(false);
    ui->dateEditDate->setReadOnly(true);
    ui->pushButtonPrev->setEnabled(false);
    ui->pushButtonNext->setEnabled(false);
}

void MainWindow::pushButtonEndPressed()
{
    {
        auto timeAssignment = *m_currentStripWidget->timeAssignments().rbegin();
        Q_ASSERT(timeAssignment.timespan == QTime(0, 0));

        auto timespan = timeBetween(m_currentStripWidget->lastTimeAssignmentStart(), ui->timeEditTime->time());

        auto reply = m_erfassung.doUpdateTimeAssignment(timeAssignment.id, m_userInfo.userId, timeAssignment.date,
                                                        timeAssignment.time, timespan,
                                                        timeAssignment.project, timeAssignment.subproject,
                                                        timeAssignment.workpackage, timeAssignment.text);

        {
            QEventLoop eventLoop;
            connect(reply.get(), SIGNAL(finished()), &eventLoop, SLOT(quit()));
            eventLoop.exec();
        }

        if(!reply->success())
        {
            QMessageBox::warning(this, tr("Could not edit time assignment!"), tr("Could not edit time assignment!") % "\n\n" % reply->message());
            m_currentStripWidget->refresh();
            return;
        }
    }

    {
        auto reply = m_erfassung.doCreateBooking(m_userInfo.userId, ui->dateEditDate->date(),
                                                 timeNormalise(ui->timeEditTime->time()), QTime(0, 0),
                                                 QStringLiteral("G"), QStringLiteral(""));

        {
            QEventLoop eventLoop;
            connect(reply.get(), &ZeiterfassungReply::finished, &eventLoop, &QEventLoop::quit);
            eventLoop.exec();
        }

        if(!reply->success())
        {
            QMessageBox::warning(this, tr("Could not create booking!"), tr("Could not create booking!") % "\n\n" % reply->message());
            m_currentStripWidget->refresh();
            return;
        }
    }

    m_currentStripWidget->refresh();
    refreshAuswertung();

    ui->actionToday->setEnabled(false);
    ui->actionRefresh->setEnabled(false);
    ui->dateEditDate->setReadOnly(true);
    ui->pushButtonPrev->setEnabled(false);
    ui->pushButtonNext->setEnabled(false);
}

void MainWindow::dateChanged(bool force)
{
    auto firstDayOfWeek = ui->dateEditDate->date().addDays(-(ui->dateEditDate->date().dayOfWeek() - 1));

    for(quint8 i = 0; i < 7; i++)
    {
        auto date = firstDayOfWeek.addDays(i);

        if(force || m_stripsWidgets[i]->date() != date)
            m_stripsWidgets[i]->setDate(date);

        if(date == ui->dateEditDate->date() && (force || m_currentStripWidget != m_stripsWidgets[i]))
        {
            if(m_currentStripWidget)
            {
                disconnect(m_currentStripWidget, &StripsWidget::minimumTimeChanged, this, &MainWindow::minimumTimeChanged);
                disconnect(m_currentStripWidget, &StripsWidget::startEnabledChanged, this, &MainWindow::startEnabledChanged);
                disconnect(m_currentStripWidget, &StripsWidget::endEnabledChanged, this, &MainWindow::endEnabledChanged);
            }

            m_currentStripWidget = m_stripsWidgets[i];

            minimumTimeChanged();
            startEnabledChanged();
            endEnabledChanged();

            connect(m_currentStripWidget, &StripsWidget::minimumTimeChanged, this, &MainWindow::minimumTimeChanged);
            connect(m_currentStripWidget, &StripsWidget::startEnabledChanged, this, &MainWindow::startEnabledChanged);
            connect(m_currentStripWidget, &StripsWidget::endEnabledChanged, this, &MainWindow::endEnabledChanged);
        }
    }

    if(force || m_auswertungDate != QDate(ui->dateEditDate->date().year(), ui->dateEditDate->date().month(), 1))
        refreshAuswertung();

    if(std::any_of(std::begin(m_stripsWidgets), std::end(m_stripsWidgets), [](StripsWidget *stripsWidget) {
        return stripsWidget->refreshing();
    }) || m_getAuswertungReply)
    {
        ui->actionToday->setEnabled(false);
        ui->actionRefresh->setEnabled(false);
        ui->dateEditDate->setReadOnly(true);
        ui->pushButtonPrev->setEnabled(false);
        ui->pushButtonNext->setEnabled(false);
    }
}

void MainWindow::openAuswertung()
{
    QTemporaryFile file(QDir::temp().absoluteFilePath(QStringLiteral("auswertungXXXXXX.pdf")));
    file.setAutoRemove(false);
    if(!file.open())
    {
        QMessageBox::warning(this, tr("Could not open auswertung!"), tr("Could not open auswertung!") % "\n\n" % file.errorString());
        return;
    }

    file.write(m_auswertung);
    file.close();

    if(!QDesktopServices::openUrl(QUrl::fromLocalFile(file.fileName())))
    {
        QMessageBox::warning(this, tr("Could not open auswertung!"), tr("Could not open default PDF viewer!"));
        return;
    }
}

void MainWindow::minimumTimeChanged()
{
    ui->timeEditTime->setMinimumTime(m_currentStripWidget->minimumTime());
}

void MainWindow::refreshingChanged()
{
    if(m_getAuswertungReply)
        return;

    {
        auto allFinished = std::none_of(std::begin(m_stripsWidgets), std::end(m_stripsWidgets), [](StripsWidget *stripsWidget){
            return stripsWidget->refreshing();
        });

        ui->actionToday->setEnabled(allFinished);
        ui->actionRefresh->setEnabled(allFinished);
        ui->dateEditDate->setReadOnly(!allFinished);
        ui->pushButtonPrev->setEnabled(allFinished);
        ui->pushButtonNext->setEnabled(allFinished);
    }
}

void MainWindow::startEnabledChanged()
{
    auto startEnabled = m_currentStripWidget->startEnabled();
    auto endEnabled = m_currentStripWidget->endEnabled();

    ui->timeEditTime->setEnabled(startEnabled || endEnabled);
    ui->pushButtonNow->setEnabled(startEnabled || endEnabled);

    ui->comboBoxProject->setEnabled(startEnabled);
    ui->comboBoxSubproject->setEnabled(startEnabled);
    ui->comboBoxWorkpackage->setEnabled(startEnabled);
    ui->comboBoxText->setEnabled(startEnabled);

    ui->pushButtonStart->setEnabled(startEnabled);
    ui->pushButtonStart->setText(endEnabled ? tr("Switch") : tr("Start"));
}

void MainWindow::endEnabledChanged()
{
    auto startEnabled = m_currentStripWidget->startEnabled();
    auto endEnabled = m_currentStripWidget->endEnabled();

    ui->timeEditTime->setEnabled(startEnabled || endEnabled);
    ui->pushButtonNow->setEnabled(startEnabled || endEnabled);

    ui->pushButtonStart->setText(endEnabled ? tr("Switch") : tr("Start"));
    ui->pushButtonEnd->setEnabled(endEnabled);
}

void MainWindow::refreshAuswertung()
{
    m_balanceLabel->setText(tr("%0: %1").arg(tr("Balance")).arg(tr("???")));
    m_holidaysLabel->setText(tr("%0: %1").arg(tr("Holidays")).arg(tr("???")));

    ui->actionAuswertung->setEnabled(false);
    m_auswertung.clear();

    m_auswertungDate = QDate(ui->dateEditDate->date().year(), ui->dateEditDate->date().month(), 1);
    m_getAuswertungReply = m_erfassung.doGetAuswertung(m_userInfo.userId, m_auswertungDate);
    connect(m_getAuswertungReply.get(), &ZeiterfassungReply::finished, this, &MainWindow::getAuswertungFinished);
}

void MainWindow::updateComboboxes()
{
    ui->comboBoxProject->clear();

    {
        auto preferedProjects = m_settings.projects();

        for(const auto &preferedProject : preferedProjects)
        {
            if(!m_projects.contains(preferedProject))
            {
                qWarning() << "cannot find project" << preferedProject;
                continue;
            }

            ui->comboBoxProject->addItem(tr("%0 (%1)").arg(m_projects.value(preferedProject)).arg(preferedProject), preferedProject);
        }

        if(preferedProjects.count())
            ui->comboBoxProject->insertSeparator(ui->comboBoxProject->count());

        for(auto iter = m_projects.constBegin(); iter != m_projects.constEnd(); iter++)
        {
            if(!preferedProjects.contains(iter.key()))
                ui->comboBoxProject->addItem(tr("%0 (%1)").arg(iter.value()).arg(iter.key()), iter.key());
        }
    }

    ui->comboBoxSubproject->clear();

    {
        auto subprojects = m_settings.subprojects();
        for(const auto &subproject : subprojects)
            ui->comboBoxSubproject->addItem(subproject);
        if(subprojects.count())
            ui->comboBoxSubproject->setCurrentText(QString());
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