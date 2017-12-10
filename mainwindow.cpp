#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QUrl>
#include <QLineEdit>
#include <QMessageBox>
#include <QStandardItem>
#include <QStringBuilder>
#include <QMenu>
#include <QEventLoop>
#include <QLabel>
#include <QDir>
#include <QTemporaryFile>
#include <QDesktopServices>
#include <QRegularExpression>
#include <QDebug>

#include "timeutils.h"
#include "zeiterfassungsettings.h"
#include "stripfactory.h"
#include "stripswidget.h"
#include "dialogs/aboutmedialog.h"
#include "dialogs/bookingdialog.h"
#include "dialogs/timeassignmentdialog.h"
#include "dialogs/settingsdialog.h"
#include "dialogs/updatedialog.h"
#include "models/bookingsmodel.h"
#include "models/timeassignmentsmodel.h"
#include "replies/getprojectsreply.h"
#include "replies/getauswertungreply.h"
#include "replies/updatebookingreply.h"
#include "replies/deletebookingreply.h"
#include "replies/createbookingreply.h"
#include "replies/updatetimeassignmentreply.h"
#include "replies/deletetimeassignmentreply.h"
#include "replies/createtimeassignmentreply.h"
#include "replies/createbookingreply.h"

MainWindow::MainWindow(ZeiterfassungSettings &settings, ZeiterfassungApi &erfassung, const ZeiterfassungApi::UserInfo &userInfo,
                       StripFactory &stripFactory, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_settings(settings),
    m_erfassung(erfassung),
    m_userInfo(userInfo),
    m_stripFactory(stripFactory),
    m_getProjectsReply(Q_NULLPTR),
    m_getAuswertungReply(Q_NULLPTR),
    m_bookingsModel(new BookingsModel(this)),
    m_timeAssignmentsModel(new TimeAssignmentsModel(this)),
    m_currentStripWidget(Q_NULLPTR)
{
    ui->setupUi(this);

    for(quint8 i = 0; i < 7; i++)
    {
        m_stripsWidgets[i] = new StripsWidget(m_erfassung, m_userInfo.userId, m_stripFactory, m_projects, ui->widgetWeek);
        connect(m_stripsWidgets[i], &StripsWidget::refreshingChanged, this, &MainWindow::refreshingChanged);
        ui->layoutWeek->addWidget(m_stripsWidgets[i], 1);
    }

    setWindowTitle(tr("Zeiterfassung - %0 (%1)").arg(m_userInfo.text).arg(m_userInfo.email));

    ui->actionQuit->setShortcut(QKeySequence::Quit);

    connect(ui->actionToday, &QAction::triggered, [=](){ ui->dateEditDate->setDate(QDate::currentDate()); });

    ui->actionRefresh->setShortcut(QKeySequence::Refresh);
    connect(ui->actionRefresh, &QAction::triggered, this, [=](){ dateChanged(true); });

    connect(ui->actionAuswertung, &QAction::triggered, this, &MainWindow::openAuswertung);

    connect(ui->actionAboutMe, &QAction::triggered, [=](){ AboutMeDialog(userInfo, this).exec(); });
    connect(ui->actionSettings, &QAction::triggered, [=](){ SettingsDialog(m_settings, this).exec(); });

    connect(ui->actionAboutQt, &QAction::triggered, [=](){ QMessageBox::aboutQt(this); });

    ui->dateEditDate->setDate(QDate::currentDate());
    connect(ui->dateEditDate, &QDateTimeEdit::dateChanged, this, [=](){ dateChanged(false); });

    connect(ui->pushButtonPrev, &QAbstractButton::pressed, [=](){ ui->dateEditDate->setDate(ui->dateEditDate->date().addDays(-1)); });
    connect(ui->pushButtonNext, &QAbstractButton::pressed, [=](){ ui->dateEditDate->setDate(ui->dateEditDate->date().addDays(1)); });

    ui->timeEditTime->setTime(timeNormalise(QTime::currentTime()));

    m_getProjectsReply = erfassung.doGetProjects(userInfo.userId, QDate::currentDate());
    connect(m_getProjectsReply, &ZeiterfassungReply::finished, this, &MainWindow::getProjectsFinished);

    ui->comboBoxProject->setMaxVisibleItems(10);

    ui->comboBoxSubproject->lineEdit()->setPlaceholderText(tr("Subproject"));
    ui->comboBoxWorkpackage->lineEdit()->setPlaceholderText(tr("Workpackage"));
    ui->comboBoxText->lineEdit()->setPlaceholderText(tr("Text"));

    updateComboboxes();

    connect(ui->pushButtonStart, &QAbstractButton::pressed, this, &MainWindow::pushButtonStartPressed);
    connect(ui->pushButtonEnd, &QAbstractButton::pressed, this, &MainWindow::pushButtonEndPressed);

    ui->treeViewBookings->setModel(m_bookingsModel);
    connect(m_bookingsModel, &BookingsModel::enabledChanged, ui->treeViewBookings, &QWidget::setEnabled);
    connect(ui->treeViewBookings, &QWidget::customContextMenuRequested, this, &MainWindow::contextMenuBooking);

    ui->treeViewTimeAssignments->setModel(m_timeAssignmentsModel);
    connect(m_timeAssignmentsModel, &TimeAssignmentsModel::enabledChanged, ui->treeViewTimeAssignments, &QWidget::setEnabled);
    connect(ui->treeViewTimeAssignments, &QWidget::customContextMenuRequested, this, &MainWindow::contextMenuTimeAssignment);

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

    m_getProjectsReply->deleteLater();
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
        m_getAuswertungReply->deleteLater();
        m_getAuswertungReply = Q_NULLPTR;
        return;
    }

    ui->actionAuswertung->setEnabled(true);
    m_auswertung = m_getAuswertungReply->auswertung();

    auto urlaubsAnspruch = tr("???");
    auto gleitzeit = tr("???");

    {
        static QRegularExpression regex(QStringLiteral("Urlaubsanspruch +([0-9]+\\.[0-9]+\\-?) +([0-9]+\\.[0-9]+\\-?)"));
        auto match = regex.match(m_auswertung);
        if(match.hasMatch())
            urlaubsAnspruch = match.captured(2);
        else
            qWarning() << "Urlaubsanspruch not found";
    }
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

    m_getAuswertungReply->deleteLater();
    m_getAuswertungReply = Q_NULLPTR;
}

void MainWindow::contextMenuBooking(const QPoint &pos)
{
    auto index = ui->treeViewBookings->indexAt(pos);

    if(!index.isValid())
    {
        QMenu menu;
        auto createAction = menu.addAction(tr("Create booking"));
        auto refreshAction = menu.addAction(QIcon(QPixmap(QStringLiteral(":/zeiterfassung/images/refresh.png"))), tr("Refresh bookings"));
        auto selectedAction = menu.exec(ui->treeViewBookings->viewport()->mapToGlobal(pos));
        if(selectedAction == createAction)
        {
            BookingDialog dialog(this);
            dialog.setTime(timeNormalise(QTime::currentTime()));
            again2:
            if(dialog.exec() == QDialog::Accepted)
            {
                auto reply = m_erfassung.doCreateBooking(m_userInfo.userId, ui->dateEditDate->date(),
                                                         dialog.getTime(), dialog.getTimespan(),
                                                         dialog.getType(), dialog.getText());

                {
                    QEventLoop eventLoop;
                    connect(reply, &ZeiterfassungReply::finished, &eventLoop, &QEventLoop::quit);
                    eventLoop.exec();
                }

                if(reply->success())
                {
                    m_currentStripWidget->refreshBookings();
                }
                else
                {
                    QMessageBox::warning(this, tr("Could not create booking!"), tr("Could not create booking!") % "\n\n" % reply->message());
                    reply->deleteLater();
                    goto again2;
                }

                reply->deleteLater();
            }
        }
        else if(selectedAction == refreshAction)
        {
            m_currentStripWidget->refreshBookings();
        }
    }
    else
    {
        auto booking = m_currentStripWidget->bookings().at(index.row());

        QMenu menu;
        auto editAction = menu.addAction(tr("Edit booking"));
        auto deleteAction = menu.addAction(tr("Delete booking"));
        auto selectedAction = menu.exec(ui->treeViewBookings->viewport()->mapToGlobal(pos));
        if(selectedAction == editAction)
        {
            BookingDialog dialog(this);
            dialog.setTime(booking.time);
            dialog.setTimespan(booking.timespan);
            dialog.setType(booking.type);
            dialog.setText(booking.text);
            again1:
            if(dialog.exec() == QDialog::Accepted)
            {
                auto reply = m_erfassung.doUpdateBooking(booking.id, m_userInfo.userId, ui->dateEditDate->date(),
                                                         dialog.getTime(), dialog.getTimespan(),
                                                         dialog.getType(), dialog.getText());

                {
                    QEventLoop eventLoop;
                    connect(reply, &ZeiterfassungReply::finished, &eventLoop, &QEventLoop::quit);
                    eventLoop.exec();
                }

                if(reply->success())
                {
                    m_currentStripWidget->refreshBookings();
                }
                else
                {
                    QMessageBox::warning(this, tr("Could not edit booking!"), tr("Could not edit booking!") % "\n\n" % reply->message());
                    reply->deleteLater();
                    goto again1;
                }

                reply->deleteLater();
            }
        }
        else if(selectedAction == deleteAction)
        {
            QMessageBox msgBox;
            msgBox.setText("Do you really want to delete the booking?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            if(msgBox.exec() == QMessageBox::Yes)
            {
                auto reply = m_erfassung.doDeleteBooking(booking.id);

                {
                    QEventLoop eventLoop;
                    connect(reply, &ZeiterfassungReply::finished, &eventLoop, &QEventLoop::quit);
                    eventLoop.exec();
                }

                if(reply->success())
                {
                    m_currentStripWidget->refreshBookings();
                }
                else
                    QMessageBox::warning(this, tr("Could not delete booking!"), tr("Could not delete booking!") % "\n\n" % reply->message());

                reply->deleteLater();
            }
        }
    }
}

void MainWindow::contextMenuTimeAssignment(const QPoint &pos)
{
    auto index = ui->treeViewTimeAssignments->indexAt(pos);

    if(!index.isValid())
    {
        QMenu menu;
        auto createAction = menu.addAction(tr("Create time assignment"));
        auto refreshAction = menu.addAction(QIcon(QPixmap(QStringLiteral(":/zeiterfassung/images/refresh.png"))), tr("Refresh time assignments"));
        auto selectedAction = menu.exec(ui->treeViewTimeAssignments->viewport()->mapToGlobal(pos));
        if(selectedAction == createAction)
        {
            TimeAssignmentDialog dialog(m_projects, m_settings, this);
            again2:
            if(dialog.exec() == QDialog::Accepted)
            {
                auto reply = m_erfassung.doCreateTimeAssignment(m_userInfo.userId, ui->dateEditDate->date(),
                                                                dialog.getTime(), dialog.getTimespan(),
                                                                dialog.getProject(), dialog.getSubproject(),
                                                                dialog.getWorkpackage(), dialog.getText());

                {
                    QEventLoop eventLoop;
                    connect(reply, &ZeiterfassungReply::finished, &eventLoop, &QEventLoop::quit);
                    eventLoop.exec();
                }

                if(reply->success())
                {
                    m_currentStripWidget->refreshTimeAssignments();
                }
                else
                {
                    QMessageBox::warning(this, tr("Could not create time assignment!"), tr("Could not create time assignment!") % "\n\n" % reply->message());
                    reply->deleteLater();
                    goto again2;
                }

                reply->deleteLater();
            }
        }
        else if(selectedAction == refreshAction)
        {
            m_currentStripWidget->refreshTimeAssignments();
        }
    }
    else
    {
        auto timeAssignment = m_currentStripWidget->timeAssignments().at(index.row());

        QMenu menu;
        auto editAction = menu.addAction(tr("Edit time assignment"));
        auto deleteAction = menu.addAction(tr("Delete time assignment"));
        auto selectedAction = menu.exec(ui->treeViewTimeAssignments->viewport()->mapToGlobal(pos));
        if(selectedAction == editAction)
        {
            TimeAssignmentDialog dialog(m_projects, m_settings, this);
            dialog.setTime(timeAssignment.time);
            dialog.setTimespan(timeAssignment.timespan);
            dialog.setProject(timeAssignment.project);
            dialog.setSubproject(timeAssignment.subproject);
            dialog.setWorkpackage(timeAssignment.workpackage);
            dialog.setText(timeAssignment.text);
            again1:
            if(dialog.exec() == QDialog::Accepted)
            {
                auto reply = m_erfassung.doUpdateTimeAssignment(timeAssignment.id, m_userInfo.userId, ui->dateEditDate->date(),
                                                                dialog.getTime(), dialog.getTimespan(),
                                                                dialog.getProject(), dialog.getSubproject(),
                                                                dialog.getWorkpackage(), dialog.getText());

                {
                    QEventLoop eventLoop;
                    connect(reply, &ZeiterfassungReply::finished, &eventLoop, &QEventLoop::quit);
                    eventLoop.exec();
                }

                if(reply->success())
                {
                    m_currentStripWidget->refreshTimeAssignments();
                }
                else
                {
                    QMessageBox::warning(this, tr("Could not edit time assignment!"), tr("Could not edit time assignment!") % "\n\n" % reply->message());
                    reply->deleteLater();
                    goto again1;
                }

                reply->deleteLater();
            }
        }
        else if(selectedAction == deleteAction)
        {
            QMessageBox msgBox;
            msgBox.setText("Do you really want to delete the time assignment?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            if(msgBox.exec() == QMessageBox::Yes)
            {
                auto reply = m_erfassung.doDeleteTimeAssignment(timeAssignment.id);

                {
                    QEventLoop eventLoop;
                    connect(reply, &ZeiterfassungReply::finished, &eventLoop, &QEventLoop::quit);
                    eventLoop.exec();
                }

                if(reply->success())
                {
                    m_currentStripWidget->refreshTimeAssignments();
                }
                else
                    QMessageBox::warning(this, tr("Could not delete time assignment!"), tr("Could not delete time assignment!") % "\n\n" % reply->message());

                reply->deleteLater();
            }
        }
    }
}

void MainWindow::pushButtonStartPressed()
{
    if(m_currentStripWidget->bookings().rbegin() == m_currentStripWidget->bookings().rend() ||
       m_currentStripWidget->bookings().rbegin()->type == QStringLiteral("G"))
    {
        auto reply = m_erfassung.doCreateBooking(m_userInfo.userId, ui->dateEditDate->date(),
                                                 timeNormalise(ui->timeEditTime->time()), QTime(0, 0),
                                                 QStringLiteral("K"), QStringLiteral(""));

        {
            QEventLoop eventLoop;
            connect(reply, &ZeiterfassungReply::finished, &eventLoop, &QEventLoop::quit);
            eventLoop.exec();
        }

        if(!reply->success())
        {
            QMessageBox::warning(this, tr("Could not create booking!"), tr("Could not create booking!") % "\n\n" % reply->message());
            m_currentStripWidget->refresh();
            reply->deleteLater();
            return;
        }

        reply->deleteLater();
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
                connect(reply, &ZeiterfassungReply::finished, &eventLoop, &QEventLoop::quit);
                eventLoop.exec();
            }

            if(reply->success())
                timeAssignmentTime = timeAdd(timeAssignmentTime, timespan);
            else
            {
                QMessageBox::warning(this, tr("Could not edit time assignment!"), tr("Could not edit time assignment!") % "\n\n" % reply->message());
                m_currentStripWidget->refresh();
                reply->deleteLater();
                return;
            }

            reply->deleteLater();
        }
    }

    auto reply = m_erfassung.doCreateTimeAssignment(m_userInfo.userId, ui->dateEditDate->date(),
                                                    timeAssignmentTime, QTime(0, 0),
                                                    ui->comboBoxProject->currentData().toString(), ui->comboBoxSubproject->currentText(),
                                                    ui->comboBoxWorkpackage->currentText(), ui->comboBoxText->currentText());

    {
        QEventLoop eventLoop;
        connect(reply, &ZeiterfassungReply::finished, &eventLoop, &QEventLoop::quit);
        eventLoop.quit();
    }

    if(!reply->success())
    {
        QMessageBox::warning(this, tr("Could not create time assignment!"), tr("Could not create time assignment!") % "\n\n" % reply->message());
        m_currentStripWidget->refresh();
        reply->deleteLater();
        return;
    }

    reply->deleteLater();

    m_settings.prependProject(ui->comboBoxProject->currentData().toString());
    m_settings.prependSubproject(ui->comboBoxSubproject->currentText());
    m_settings.prependWorkpackage(ui->comboBoxWorkpackage->currentText());
    m_settings.prependText(ui->comboBoxText->currentText());

    updateComboboxes();

    m_currentStripWidget->refresh();
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
            connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
            eventLoop.exec();
        }

        if(!reply->success())
        {
            QMessageBox::warning(this, tr("Could not edit time assignment!"), tr("Could not edit time assignment!") % "\n\n" % reply->message());
            m_currentStripWidget->refresh();
            reply->deleteLater();
            return;
        }

        reply->deleteLater();
    }

    {
        auto reply = m_erfassung.doCreateBooking(m_userInfo.userId, ui->dateEditDate->date(),
                                                 timeNormalise(ui->timeEditTime->time()), QTime(0, 0),
                                                 QStringLiteral("G"), QStringLiteral(""));

        {
            QEventLoop eventLoop;
            connect(reply, &ZeiterfassungReply::finished, &eventLoop, &QEventLoop::quit);
            eventLoop.exec();
        }

        if(!reply->success())
        {
            QMessageBox::warning(this, tr("Could not create booking!"), tr("Could not create booking!") % "\n\n" % reply->message());
            m_currentStripWidget->refresh();
            reply->deleteLater();
            return;
        }

        reply->deleteLater();
    }

    m_currentStripWidget->refresh();
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

            m_bookingsModel->setStripsWidget(m_currentStripWidget);
            m_timeAssignmentsModel->setStripsWidget(m_currentStripWidget);

            minimumTimeChanged();
            startEnabledChanged();
            endEnabledChanged();

            connect(m_currentStripWidget, &StripsWidget::minimumTimeChanged, this, &MainWindow::minimumTimeChanged);
            connect(m_currentStripWidget, &StripsWidget::startEnabledChanged, this, &MainWindow::startEnabledChanged);
            connect(m_currentStripWidget, &StripsWidget::endEnabledChanged, this, &MainWindow::endEnabledChanged);
        }
    }

    auto auswertungDate = QDate(ui->dateEditDate->date().year(), ui->dateEditDate->date().month(), 1);
    if(force || m_auswertungDate != auswertungDate)
    {
        m_balanceLabel->setText(tr("%0: %1").arg(tr("Balance")).arg(tr("???")));
        m_holidaysLabel->setText(tr("%0: %1").arg(tr("Holidays")).arg(tr("???")));

        ui->actionAuswertung->setEnabled(false);
        m_auswertung.clear();

        m_auswertungDate = auswertungDate;
        m_getAuswertungReply = m_erfassung.doGetAuswertung(m_userInfo.userId, auswertungDate);
        connect(m_getAuswertungReply, &ZeiterfassungReply::finished, this, &MainWindow::getAuswertungFinished);
    }

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
    QTemporaryFile file(QDir::temp().absoluteFilePath("auswertungXXXXXX.pdf"));
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

    ui->timeEditTime->setEnabled(startEnabled ||endEnabled);

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

    ui->timeEditTime->setEnabled(startEnabled ||endEnabled);

    ui->pushButtonStart->setText(endEnabled ? tr("Switch") : tr("Start"));
    ui->pushButtonEnd->setEnabled(endEnabled);
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

            ui->comboBoxProject->addItem(m_projects.value(preferedProject) % " (" % preferedProject % ')', preferedProject);
        }

        if(preferedProjects.count())
            ui->comboBoxProject->insertSeparator(ui->comboBoxProject->count());

        for(auto iter = m_projects.constBegin(); iter != m_projects.constEnd(); iter++)
        {
            if(!preferedProjects.contains(iter.key()))
                ui->comboBoxProject->addItem(iter.value() % " (" % iter.key() % ')', iter.key());
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
