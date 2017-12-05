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
#include "dialogs/bookingdialog.h"
#include "dialogs/timeassignmentdialog.h"
#include "dialogs/settingsdialog.h"
#include "strips/bookingstrip.h"
#include "strips/timeassignmentstrip.h"
#include "models/bookingsmodel.h"
#include "models/timeassignmentsmodel.h"

MainWindow::MainWindow(ZeiterfassungSettings &settings, Zeiterfassung &erfassung, const Zeiterfassung::UserInfo &userInfo, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_settings(settings),
    m_erfassung(erfassung),
    m_userInfo(userInfo),
    m_bookingsModel(new BookingsModel(erfassung, this)),
    m_timeAssignmentsModel(new TimeAssignmentsModel(erfassung, this)),
    m_flag(false)
{
    ui->setupUi(this);

    setWindowTitle(tr("Zeiterfassung - %0 (%1)").arg(m_userInfo.text).arg(m_userInfo.email));

    ui->actionQuit->setShortcut(QKeySequence::Quit);

    connect(ui->actionToday, &QAction::triggered, [=](){ ui->dateEditDate->setDate(QDate::currentDate()); });

    ui->actionRefresh->setShortcut(QKeySequence::Refresh);
    connect(ui->actionRefresh, &QAction::triggered, this, [=](){ refresh(true); });

    connect(ui->actionAuswertung, &QAction::triggered, [=](){
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
    });

    connect(ui->actionAboutMe, &QAction::triggered, [=](){ AboutMeDialog(userInfo, this).exec(); });
    connect(ui->actionSettings, &QAction::triggered, [=](){ SettingsDialog(m_settings, this).exec(); });

    connect(ui->actionAboutQt, &QAction::triggered, [=](){ QMessageBox::aboutQt(this); });

    ui->dateEditDate->setDate(QDate::currentDate());
    connect(ui->dateEditDate, &QDateTimeEdit::dateChanged, this, [=](){ refresh(); });

    connect(ui->pushButtonPrev, &QAbstractButton::pressed, [=](){ ui->dateEditDate->setDate(ui->dateEditDate->date().addDays(-1)); });
    connect(ui->pushButtonNext, &QAbstractButton::pressed, [=](){ ui->dateEditDate->setDate(ui->dateEditDate->date().addDays(1)); });

    ui->timeEditTime->setTime(timeNormalise(QTime::currentTime()));

    connect(&m_erfassung, &Zeiterfassung::getProjectsFinished,
            this, &MainWindow::getProjekctsFinished);
    erfassung.doGetProjects(userInfo.userId, QDate::currentDate());

    ui->comboBoxProject->setMaxVisibleItems(10);

    ui->comboBoxSubproject->lineEdit()->setPlaceholderText(tr("Subproject"));
    ui->comboBoxWorkpackage->lineEdit()->setPlaceholderText(tr("Workpackage"));
    ui->comboBoxText->lineEdit()->setPlaceholderText(tr("Text"));

    updateComboboxes();

    connect(ui->pushButtonStart, &QAbstractButton::pressed, this, &MainWindow::pushButtonStartPressed);
    connect(ui->pushButtonEnd, &QAbstractButton::pressed, this, &MainWindow::pushButtonEndPressed);

    ui->treeViewBookings->setModel(m_bookingsModel);
    ui->treeViewTimeAssignments->setModel(m_timeAssignmentsModel);

    connect(ui->treeViewBookings, &QWidget::customContextMenuRequested,
            this,                 &MainWindow::contextMenuBooking);
    connect(ui->treeViewTimeAssignments, &QWidget::customContextMenuRequested,
            this,                        &MainWindow::contextMenuTimeAssignment);

    ui->statusbar->addPermanentWidget(m_workingTimeLabel = new QLabel(ui->statusbar));
    m_workingTimeLabel->setFrameShape(QFrame::Panel);
    m_workingTimeLabel->setFrameShadow(QFrame::Sunken);
    ui->statusbar->addPermanentWidget(m_balanceLabel = new QLabel(ui->statusbar));
    m_balanceLabel->setFrameShape(QFrame::Panel);
    m_balanceLabel->setFrameShadow(QFrame::Sunken);
    ui->statusbar->addPermanentWidget(m_holidaysLabel = new QLabel(ui->statusbar));
    m_holidaysLabel->setFrameShape(QFrame::Panel);
    m_holidaysLabel->setFrameShadow(QFrame::Sunken);

    refresh(true);
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

void MainWindow::refresh(bool forceAuswertung)
{
    ui->actionToday->setEnabled(false);
    ui->actionRefresh->setEnabled(false);
    ui->dateEditDate->setReadOnly(true);
    ui->pushButtonPrev->setEnabled(false);
    ui->pushButtonNext->setEnabled(false);
    ui->timeEditTime->setEnabled(false);
    ui->comboBoxProject->setEnabled(false);
    ui->comboBoxSubproject->setEnabled(false);
    ui->comboBoxWorkpackage->setEnabled(false);
    ui->comboBoxText->setEnabled(false);
    ui->pushButtonStart->setEnabled(false);
    ui->pushButtonEnd->setEnabled(false);
    ui->treeViewBookings->setEnabled(false);
    ui->treeViewTimeAssignments->setEnabled(false);

    m_workingTimeLabel->setText(tr("%0: %1").arg(tr("Assigned time")).arg(tr("???")));

    auto waitForBookings = m_bookingsModel->refresh(m_userInfo.userId, ui->dateEditDate->date(), ui->dateEditDate->date());
    if(waitForBookings)
    {
        connect(m_bookingsModel, &BookingsModel::refreshFinished,
                this,             &MainWindow::refreshBookingsFinished);
    }

    auto waitForTimeAssignments = m_timeAssignmentsModel->refresh(m_userInfo.userId, ui->dateEditDate->date(), ui->dateEditDate->date());
    if(waitForTimeAssignments)
    {
        connect(m_timeAssignmentsModel, &TimeAssignmentsModel::refreshFinished,
                this,                &MainWindow::refreshTimeAssignmentsFinished);
    }

    if(!waitForBookings || !waitForTimeAssignments)
        QMessageBox::warning(this, tr("Unknown error occured!"), tr("Unknown error occured!"));

    if(waitForBookings || waitForTimeAssignments)
        m_flag = waitForBookings == waitForTimeAssignments;
    else
    {
        ui->actionToday->setEnabled(true);
        ui->actionRefresh->setEnabled(true);
        ui->dateEditDate->setReadOnly(false);
        ui->pushButtonPrev->setEnabled(true);
        ui->pushButtonNext->setEnabled(true);
    }

    clearStrips();

    auto auswertungDate = QDate(ui->dateEditDate->date().year(), ui->dateEditDate->date().month(), 1);
    if(forceAuswertung || m_auswertungDate != auswertungDate)
    {
        m_balanceLabel->setText(tr("%0: %1").arg(tr("Balance")).arg(tr("???")));
        m_holidaysLabel->setText(tr("%0: %1").arg(tr("Holidays")).arg(tr("???")));

        ui->actionAuswertung->setEnabled(false);
        m_auswertung.clear();

        if(m_erfassung.doGetAuswertung(m_userInfo.userId, auswertungDate))
        {
            m_auswertungDate = auswertungDate;
            connect(&m_erfassung, &Zeiterfassung::getAuswertungFinished,
                    this,         &MainWindow::getAuswertungFinished);

        }
        else
        {
            m_auswertungDate = QDate();
            QMessageBox::warning(this, tr("Unknown error occured!"), tr("Unknown error occured!"));
        }
    }
}

void MainWindow::getProjekctsFinished(bool success, const QString &message, const QVector<Zeiterfassung::Project> &projects)
{
    disconnect(&m_erfassung, &Zeiterfassung::getProjectsFinished,
               this, &MainWindow::getProjekctsFinished);

    if(!success)
    {
        QMessageBox::warning(this, tr("Could not load bookings!"), tr("Could not load bookings!") % "\n\n" % message);
        return;
    }

    m_projects.clear();

    for(const auto &project : projects)
        m_projects.insert(project.value, project.label);

    updateComboboxes();
}

void MainWindow::getAuswertungFinished(bool success, const QString &message, const QByteArray &content)
{
    disconnect(&m_erfassung, &Zeiterfassung::getAuswertungFinished,
               this,         &MainWindow::getAuswertungFinished);

    if(!success)
    {
        m_auswertungDate = QDate();
        QMessageBox::warning(this, tr("Could not load Auswertung!"), tr("Could not load Auswertung!") % "\n\n" % message);
        return;
    }

    ui->actionAuswertung->setEnabled(true);
    m_auswertung = content;

    auto urlaubsAnspruch = tr("???");
    auto gleitzeit = tr("???");

    {
        static QRegularExpression regex(QStringLiteral("Urlaubsanspruch +([0-9]+\\.[0-9]+\\-?) +([0-9]+\\.[0-9]+\\-?)"));
        auto match = regex.match(content);
        if(match.hasMatch())
            urlaubsAnspruch = match.captured(2);
        else
            qWarning() << "Urlaubsanspruch not found";
    }
    {
        static QRegularExpression regex(QStringLiteral("Gleitzeit +([0-9]+\\:[0-9]+\\-?) +([0-9]+\\:[0-9]+\\-?)"));
        auto match = regex.match(content);
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
}

void MainWindow::refreshBookingsFinished(bool success, const QString &message)
{
    disconnect(m_bookingsModel, &BookingsModel::refreshFinished,
               this,             &MainWindow::refreshBookingsFinished);

    if(success)
        ui->treeViewBookings->setEnabled(true);

    if(m_flag)
        m_flag = false;
    else
        validateEntries();

    if(!success)
        QMessageBox::warning(Q_NULLPTR, tr("Could not load bookings!"), tr("Could not load bookings!") % "\n\n" % message);
}

void MainWindow::refreshTimeAssignmentsFinished(bool success, const QString &message)
{
    disconnect(m_timeAssignmentsModel, &TimeAssignmentsModel::refreshFinished,
               this,                &MainWindow::refreshTimeAssignmentsFinished);

    if(success)
        ui->treeViewTimeAssignments->setEnabled(true);

    if(m_flag)
        m_flag = false;
    else
        validateEntries();

    if(!success)
        QMessageBox::warning(Q_NULLPTR, tr("Could not load time assignments!"), tr("Could not load time assignments!") % "\n\n" % message);
}

void MainWindow::contextMenuBooking(const QPoint &pos)
{
    auto index = ui->treeViewBookings->indexAt(pos);

    if(index.isValid())
    {
        auto booking = m_bookingsModel->getBooking(index);

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
                EventLoopWithStatus eventLoop;
                connect(&m_erfassung, &Zeiterfassung::updateBookingFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

                m_erfassung.doUpdateBooking(booking.id, m_userInfo.userId, ui->dateEditDate->date(),
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
                    ui->comboBoxProject->setEnabled(false);
                    ui->comboBoxSubproject->setEnabled(false);
                    ui->comboBoxWorkpackage->setEnabled(false);
                    ui->comboBoxText->setEnabled(false);
                    ui->pushButtonStart->setEnabled(false);
                    ui->pushButtonEnd->setEnabled(false);
                    ui->treeViewBookings->setEnabled(false);

                    clearStrips();

                    if(m_bookingsModel->refresh(m_userInfo.userId, ui->dateEditDate->date(), ui->dateEditDate->date()))
                    {
                        connect(m_bookingsModel, &BookingsModel::refreshFinished,
                                this,             &MainWindow::refreshBookingsFinished);
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
                    QMessageBox::warning(this, tr("Could not edit booking!"), tr("Could not edit booking!") % "\n\n" % eventLoop.message());
                    goto again1;
                }
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
                EventLoopWithStatus eventLoop;
                connect(&m_erfassung, &Zeiterfassung::deleteBookingFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

                m_erfassung.doDeleteBooking(booking.id);
                eventLoop.exec();

                if(eventLoop.success())
                {
                    ui->actionToday->setEnabled(false);
                    ui->actionRefresh->setEnabled(false);
                    ui->dateEditDate->setReadOnly(true);
                    ui->pushButtonPrev->setEnabled(false);
                    ui->pushButtonNext->setEnabled(false);
                    ui->timeEditTime->setEnabled(false);
                    ui->comboBoxProject->setEnabled(false);
                    ui->comboBoxSubproject->setEnabled(false);
                    ui->comboBoxWorkpackage->setEnabled(false);
                    ui->comboBoxText->setEnabled(false);
                    ui->pushButtonStart->setEnabled(false);
                    ui->pushButtonEnd->setEnabled(false);
                    ui->treeViewBookings->setEnabled(false);

                    clearStrips();

                    if(m_bookingsModel->refresh(m_userInfo.userId, ui->dateEditDate->date(), ui->dateEditDate->date()))
                    {
                        connect(m_bookingsModel, &BookingsModel::refreshFinished,
                                this,             &MainWindow::refreshBookingsFinished);
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
                    QMessageBox::warning(this, tr("Could not delete booking!"), tr("Could not delete booking!") % "\n\n" % eventLoop.message());
            }
        }
    }
    else
    {
        QMenu menu;
        auto createAction = menu.addAction(tr("Create booking"));
        auto selectedAction = menu.exec(ui->treeViewBookings->viewport()->mapToGlobal(pos));
        if(selectedAction == createAction)
        {
            BookingDialog dialog(this);
            dialog.setTime(timeNormalise(QTime::currentTime()));
            again2:
            if(dialog.exec() == QDialog::Accepted)
            {
                EventLoopWithStatus eventLoop;
                connect(&m_erfassung, &Zeiterfassung::createBookingFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

                m_erfassung.doCreateBooking(m_userInfo.userId, ui->dateEditDate->date(),
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
                    ui->comboBoxProject->setEnabled(false);
                    ui->comboBoxSubproject->setEnabled(false);
                    ui->comboBoxWorkpackage->setEnabled(false);
                    ui->comboBoxText->setEnabled(false);
                    ui->pushButtonStart->setEnabled(false);
                    ui->pushButtonEnd->setEnabled(false);
                    ui->treeViewBookings->setEnabled(false);

                    clearStrips();

                    if(m_bookingsModel->refresh(m_userInfo.userId, ui->dateEditDate->date(), ui->dateEditDate->date()))
                    {
                        connect(m_bookingsModel, &BookingsModel::refreshFinished,
                                this,             &MainWindow::refreshBookingsFinished);
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
                    QMessageBox::warning(this, tr("Could not create booking!"), tr("Could not create booking!") % "\n\n" % eventLoop.message());
                    goto again2;
                }
            }
        }
    }
}

void MainWindow::contextMenuTimeAssignment(const QPoint &pos)
{
    auto index = ui->treeViewTimeAssignments->indexAt(pos);

    if(index.isValid())
    {
        auto timeAssignment = m_timeAssignmentsModel->getTimeAssignment(index);

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
                EventLoopWithStatus eventLoop;
                connect(&m_erfassung, &Zeiterfassung::updateTimeAssignmentFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

                m_erfassung.doUpdateTimeAssignment(timeAssignment.id, m_userInfo.userId, ui->dateEditDate->date(),
                                               dialog.getTime(), dialog.getTimespan(),
                                               dialog.getProject(), dialog.getSubproject(),
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
                    ui->comboBoxProject->setEnabled(false);
                    ui->comboBoxSubproject->setEnabled(false);
                    ui->comboBoxWorkpackage->setEnabled(false);
                    ui->comboBoxText->setEnabled(false);
                    ui->pushButtonStart->setEnabled(false);
                    ui->pushButtonEnd->setEnabled(false);
                    ui->treeViewTimeAssignments->setEnabled(false);

                    m_settings.prependProject(dialog.getProject());
                    m_settings.prependSubproject(dialog.getSubproject());
                    m_settings.prependWorkpackage(dialog.getWorkpackage());
                    m_settings.prependText(dialog.getText());

                    clearStrips();

                    if(m_timeAssignmentsModel->refresh(m_userInfo.userId, ui->dateEditDate->date(), ui->dateEditDate->date()))
                    {
                        connect(m_timeAssignmentsModel, &TimeAssignmentsModel::refreshFinished,
                                this,                &MainWindow::refreshTimeAssignmentsFinished);
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
                    QMessageBox::warning(this, tr("Could not edit time assignment!"), tr("Could not edit time assignment!") % "\n\n" % eventLoop.message());
                    goto again1;
                }
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
                EventLoopWithStatus eventLoop;
                connect(&m_erfassung, &Zeiterfassung::deleteTimeAssignmentFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

                m_erfassung.doDeleteTimeAssignment(timeAssignment.id);
                eventLoop.exec();

                if(eventLoop.success())
                {
                    ui->actionToday->setEnabled(false);
                    ui->actionRefresh->setEnabled(false);
                    ui->dateEditDate->setReadOnly(true);
                    ui->pushButtonPrev->setEnabled(false);
                    ui->pushButtonNext->setEnabled(false);
                    ui->timeEditTime->setEnabled(false);
                    ui->comboBoxProject->setEnabled(false);
                    ui->comboBoxSubproject->setEnabled(false);
                    ui->comboBoxWorkpackage->setEnabled(false);
                    ui->comboBoxText->setEnabled(false);
                    ui->pushButtonStart->setEnabled(false);
                    ui->pushButtonEnd->setEnabled(false);
                    ui->treeViewTimeAssignments->setEnabled(false);

                    clearStrips();

                    if(m_timeAssignmentsModel->refresh(m_userInfo.userId, ui->dateEditDate->date(), ui->dateEditDate->date()))
                    {
                        connect(m_timeAssignmentsModel, &TimeAssignmentsModel::refreshFinished,
                                this,                &MainWindow::refreshTimeAssignmentsFinished);
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
                    QMessageBox::warning(this, tr("Could not delete time assignment!"), tr("Could not delete time assignment!") % "\n\n" % eventLoop.message());
            }
        }
    }
    else
    {
        QMenu menu;
        auto createAction = menu.addAction(tr("Create time assignment"));
        auto selectedAction = menu.exec(ui->treeViewTimeAssignments->viewport()->mapToGlobal(pos));
        if(selectedAction == createAction)
        {
            TimeAssignmentDialog dialog(m_projects, m_settings, this);
            again2:
            if(dialog.exec() == QDialog::Accepted)
            {
                EventLoopWithStatus eventLoop;
                connect(&m_erfassung, &Zeiterfassung::createTimeAssignmentFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

                m_erfassung.doCreateTimeAssignment(m_userInfo.userId, ui->dateEditDate->date(),
                                               dialog.getTime(), dialog.getTimespan(),
                                               dialog.getProject(), dialog.getSubproject(),
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
                    ui->comboBoxProject->setEnabled(false);
                    ui->comboBoxSubproject->setEnabled(false);
                    ui->comboBoxWorkpackage->setEnabled(false);
                    ui->comboBoxText->setEnabled(false);
                    ui->pushButtonStart->setEnabled(false);
                    ui->pushButtonEnd->setEnabled(false);
                    ui->treeViewTimeAssignments->setEnabled(false);

                    m_settings.prependProject(dialog.getProject());
                    m_settings.prependSubproject(dialog.getSubproject());
                    m_settings.prependWorkpackage(dialog.getWorkpackage());
                    m_settings.prependText(dialog.getText());

                    clearStrips();

                    if(m_timeAssignmentsModel->refresh(m_userInfo.userId, ui->dateEditDate->date(), ui->dateEditDate->date()))
                    {
                        connect(m_timeAssignmentsModel, &TimeAssignmentsModel::refreshFinished,
                                this,                &MainWindow::refreshTimeAssignmentsFinished);
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
                    QMessageBox::warning(this, tr("Could not create time assignment!"), tr("Could not create time assignment!") % "\n\n" % eventLoop.message());
                    goto again2;
                }
            }
        }
    }
}

void MainWindow::pushButtonStartPressed()
{
    if(m_bookingsModel->rbegin() == m_bookingsModel->rend() ||
       m_bookingsModel->rbegin()->type == QStringLiteral("G"))
    {
        EventLoopWithStatus eventLoop;
        connect(&m_erfassung, &Zeiterfassung::createBookingFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

        m_erfassung.doCreateBooking(m_userInfo.userId, ui->dateEditDate->date(),
                                    timeNormalise(ui->timeEditTime->time()), QTime(0, 0),
                                    QStringLiteral("K"), QStringLiteral(""));
        eventLoop.exec();

        if(!eventLoop.success())
        {
            QMessageBox::warning(this, tr("Could not create booking!"), tr("Could not create booking!") % "\n\n" % eventLoop.message());
            refresh(true);
            return;
        }
    }

    if(m_timeAssignmentsModel->rbegin() != m_timeAssignmentsModel->rend())
    {
        auto timeAssignment = *m_timeAssignmentsModel->rbegin();
        if(timeAssignment.timespan == QTime(0, 0))
        {
            EventLoopWithStatus eventLoop;
            connect(&m_erfassung, &Zeiterfassung::updateTimeAssignmentFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

            auto timespan = timeBetween(m_lastTimeAssignmentStart, ui->timeEditTime->time());

            m_erfassung.doUpdateTimeAssignment(timeAssignment.id, m_userInfo.userId, timeAssignment.date,
                                           timeAssignment.time, timespan,
                                           timeAssignment.project, timeAssignment.subproject,
                                           timeAssignment.workpackage, timeAssignment.text);
            eventLoop.exec();

            if(eventLoop.success())
                m_timeAssignmentTime = timeAdd(m_timeAssignmentTime, timespan);
            else
            {
                QMessageBox::warning(this, tr("Could not edit time assignment!"), tr("Could not edit time assignment!") % "\n\n" % eventLoop.message());
                refresh(true);
                return;
            }
        }
    }

    EventLoopWithStatus eventLoop;
    connect(&m_erfassung, &Zeiterfassung::createTimeAssignmentFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

    m_erfassung.doCreateTimeAssignment(m_userInfo.userId, ui->dateEditDate->date(),
                                   m_timeAssignmentTime, QTime(0, 0),
                                   ui->comboBoxProject->currentData().toString(), ui->comboBoxSubproject->currentText(),
                                   ui->comboBoxWorkpackage->currentText(), ui->comboBoxText->currentText());
    eventLoop.exec();

    if(!eventLoop.success())
    {
        QMessageBox::warning(this, tr("Could not create time assignment!"), tr("Could not create time assignment!") % "\n\n" % eventLoop.message());
        refresh(true);
        return;
    }

    m_settings.prependProject(ui->comboBoxProject->currentData().toString());
    m_settings.prependSubproject(ui->comboBoxSubproject->currentText());
    m_settings.prependWorkpackage(ui->comboBoxWorkpackage->currentText());
    m_settings.prependText(ui->comboBoxText->currentText());

    updateComboboxes();

    refresh(true);
}

void MainWindow::pushButtonEndPressed()
{
    {
        EventLoopWithStatus eventLoop;
        connect(&m_erfassung, &Zeiterfassung::createBookingFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

        m_erfassung.doCreateBooking(m_userInfo.userId, ui->dateEditDate->date(),
                                    timeNormalise(ui->timeEditTime->time()), QTime(0, 0),
                                    QStringLiteral("G"), QStringLiteral(""));
        eventLoop.exec();

        if(!eventLoop.success())
        {
            QMessageBox::warning(this, tr("Could not create booking!"), tr("Could not create booking!") % "\n\n" % eventLoop.message());
            refresh(true);
            return;
        }
    }

    {
        auto timeAssignment = *m_timeAssignmentsModel->rbegin();
        Q_ASSERT(timeAssignment.timespan == QTime(0, 0));

        EventLoopWithStatus eventLoop;
        connect(&m_erfassung, &Zeiterfassung::updateTimeAssignmentFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

        auto timespan = timeBetween(m_lastTimeAssignmentStart, ui->timeEditTime->time());

        m_erfassung.doUpdateTimeAssignment(timeAssignment.id, m_userInfo.userId, timeAssignment.date,
                                       timeAssignment.time, timespan,
                                       timeAssignment.project, timeAssignment.subproject,
                                       timeAssignment.workpackage, timeAssignment.text);
        eventLoop.exec();

        if(eventLoop.success())
            m_timeAssignmentTime = timeAdd(m_timeAssignmentTime, timespan);
        else
        {
            QMessageBox::warning(this, tr("Could not edit time assignment!"), tr("Could not edit time assignment!") % "\n\n" % eventLoop.message());
            refresh(true);
            return;
        }
    }

    refresh(true);
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

    if(!ui->treeViewBookings->isEnabled())
        return;

    if(!ui->treeViewTimeAssignments->isEnabled())
        return;

    auto bookingsIter = m_bookingsModel->constBegin();
    auto timeAssignmentsIter = m_timeAssignmentsModel->constBegin();

    m_timeAssignmentTime = QTime(0, 0);
    auto bookingTimespan = QTime(0, 0);

    const Zeiterfassung::Booking *lastBooking = Q_NULLPTR;
    const Zeiterfassung::TimeAssignment *lastTimeAssignment = Q_NULLPTR;

    QString errorMessage;

    while(true)
    {
        if(bookingsIter == m_bookingsModel->constEnd() &&
           timeAssignmentsIter == m_timeAssignmentsModel->constEnd())
        {
            goto after;
        }

        if(bookingsIter == m_bookingsModel->constEnd())
        {
            errorMessage = tr("Missing booking!");
            goto after;
        }

        auto startBooking = *bookingsIter++;
        if(startBooking.type != QStringLiteral("K"))
        {
            errorMessage = tr("Expected start booking, instead got type %0\nBooking ID: %1")
                    .arg(startBooking.type)
                    .arg(startBooking.id);
            goto after;
        }

        if(lastBooking)
        {
            auto label = new QLabel(tr("%0: %1")
                                    .arg(tr("Break"))
                                    .arg(tr("%0h").arg(timeBetween(lastBooking->time, startBooking.time).toString(QStringLiteral("HH:mm")))),
                                    ui->scrollAreaWidgetContents);
            ui->verticalLayout2->addWidget(label);
            label->setMinimumHeight(20);
            label->setMaximumHeight(20);
        }

        lastBooking = &startBooking;

        m_lastTimeAssignmentStart = startBooking.time;
        ui->verticalLayout2->addWidget(new BookingStrip(startBooking.id, startBooking.time, startBooking.type, m_settings, ui->scrollAreaWidgetContents));

        if(timeAssignmentsIter == m_timeAssignmentsModel->constEnd())
        {
            errorMessage = tr("Missing time assignment!");
            goto after;
        }

        auto timeAssignment = *timeAssignmentsIter++;
        if(timeAssignment.time != m_timeAssignmentTime)
        {
            errorMessage = tr("Expected %0 but received %1 in time assignment.\nTime assignment ID: %2")
                    .arg(m_timeAssignmentTime.toString("HH:mm:ss"))
                    .arg(timeAssignment.time.toString("HH:mm:ss"))
                    .arg(timeAssignment.id);
            goto after;
        }

        lastTimeAssignment = &timeAssignment;

        ui->verticalLayout2->addWidget(new TimeAssignmentStrip(timeAssignment.id, timeAssignment.timespan, buildProjectString(timeAssignment.project),
                                                           timeAssignment.subproject, timeAssignment.workpackage, timeAssignment.text,
                                                           m_settings, ui->scrollAreaWidgetContents));

        if(timeAssignment.timespan == QTime(0, 0))
        {
            if(bookingsIter != m_bookingsModel->constEnd())
            {
                errorMessage = tr("There is another booking after an unfinished time assignment.\nBooking ID: %0\nTime assignment ID: %1")
                        .arg(bookingsIter->id)
                        .arg(timeAssignment.id);
                goto after;
            }

            if(timeAssignmentsIter != m_timeAssignmentsModel->constEnd())
            {
                errorMessage = tr("There is another time assignment after an unfinished time assignment.\nTime assignment ID: %0\nTime assignment ID: %1")
                        .arg(timeAssignmentsIter->id)
                        .arg(timeAssignment.id);
                goto after;
            }

            ui->timeEditTime->setMinimumTime(timeAdd(m_lastTimeAssignmentStart, QTime(0, 1)));
            ui->pushButtonStart->setText(tr("Switch"));
            ui->pushButtonEnd->setEnabled(true);
            goto after;
        }
        else
        {
            m_timeAssignmentTime = timeAdd(m_timeAssignmentTime, timeAssignment.timespan);
            m_lastTimeAssignmentStart = timeAdd(m_lastTimeAssignmentStart, timeAssignment.timespan);

            if(bookingsIter == m_bookingsModel->constEnd())
            {
                while(true)
                {
                    if(timeAssignmentsIter == m_timeAssignmentsModel->constEnd())
                    {
                        errorMessage = tr("The last time assignment is finished without end booking\nTime assignment ID: %0")
                                .arg(timeAssignment.id);
                        goto after;
                    }

                    timeAssignment = *timeAssignmentsIter++;
                    if(timeAssignment.time != m_timeAssignmentTime)
                    {
                        errorMessage = tr("Expected %0 but received %1 in time assignment.\nTime assignment ID: %2")
                                .arg(m_timeAssignmentTime.toString("HH:mm:ss"))
                                .arg(timeAssignment.time.toString("HH:mm:ss"))
                                .arg(timeAssignment.id);
                        goto after;
                    }

                    lastTimeAssignment = &timeAssignment;

                    ui->verticalLayout2->addWidget(new TimeAssignmentStrip(timeAssignment.id, timeAssignment.timespan, buildProjectString(timeAssignment.project),
                                                                       timeAssignment.subproject, timeAssignment.workpackage, timeAssignment.text,
                                                                       m_settings, ui->scrollAreaWidgetContents));

                    if(timeAssignment.timespan == QTime(0, 0))
                    {
                        if(timeAssignmentsIter != m_timeAssignmentsModel->constEnd())
                        {
                            errorMessage = tr("There is another time assignment after an unfinished time assignment.\n"
                                              "Time assignment ID: %0\nTime assignment ID: %1")
                                    .arg(timeAssignment.id)
                                    .arg(timeAssignmentsIter->id);
                            goto after;
                        }

                        ui->timeEditTime->setMinimumTime(timeAdd(m_lastTimeAssignmentStart, QTime(0, 1)));
                        ui->pushButtonStart->setText(tr("Switch"));
                        ui->pushButtonEnd->setEnabled(true);
                        goto after;
                    }
                    else
                    {
                        m_timeAssignmentTime = timeAdd(m_timeAssignmentTime, timeAssignment.timespan);
                        m_lastTimeAssignmentStart = timeAdd(m_lastTimeAssignmentStart, timeAssignment.timespan);
                    }
                }
            }
            else
            {
                auto endBooking = *bookingsIter++;
                if(endBooking.type != QStringLiteral("G"))
                {
                    errorMessage = tr("Expected end booking, instead got type %0\nBooking ID: %1")
                            .arg(endBooking.type)
                            .arg(endBooking.id);
                    goto after;
                }

                lastBooking = &endBooking;

                bookingTimespan = timeAdd(bookingTimespan, timeBetween(startBooking.time, endBooking.time));
                ui->timeEditTime->setMinimumTime(timeAdd(endBooking.time, QTime(0, 1)));

                while(m_timeAssignmentTime < bookingTimespan)
                {
                    if(timeAssignmentsIter == m_timeAssignmentsModel->constEnd())
                    {
                        errorMessage = tr("Missing time assignment! Missing: %0h")
                                .arg(timeBetween(m_timeAssignmentTime, bookingTimespan).toString("HH:mm:ss"));

                        {
                            auto label = new QLabel(errorMessage, ui->scrollAreaWidgetContents);
                            ui->verticalLayout2->addWidget(label);
                            label->setMinimumHeight(20);
                            label->setMaximumHeight(20);
                        }

                        ui->verticalLayout2->addWidget(new BookingStrip(endBooking.id, endBooking.time, endBooking.type, m_settings, ui->scrollAreaWidgetContents));

                        goto after;
                    }

                    timeAssignment = *timeAssignmentsIter++;
                    if(timeAssignment.time != m_timeAssignmentTime)
                    {
                        errorMessage = tr("Expected %0 but received %1 in time assignment.\nTime assignment ID: %2")
                                .arg(m_timeAssignmentTime.toString("HH:mm:ss"))
                                .arg(timeAssignment.time.toString("HH:mm:ss"))
                                .arg(timeAssignment.id);
                        goto after;
                    }

                    lastTimeAssignment = &timeAssignment;

                    ui->verticalLayout2->addWidget(new TimeAssignmentStrip(timeAssignment.id, timeAssignment.timespan, buildProjectString(timeAssignment.project),
                                                                       timeAssignment.subproject, timeAssignment.workpackage, timeAssignment.text,
                                                                       m_settings, ui->scrollAreaWidgetContents));

                    if(timeAssignment.timespan == QTime(0, 0))
                    {
                        if(bookingsIter != m_bookingsModel->constEnd())
                        {
                            errorMessage = tr("There is another booking after an unfinished time assignment.\n"
                                              "Booking ID: %0\nTime assignment ID: %1")
                                    .arg(bookingsIter->id)
                                    .arg(timeAssignment.id);
                            goto after;
                        }

                        if(timeAssignmentsIter != m_timeAssignmentsModel->constEnd())
                        {
                            errorMessage = tr("There is another time assignment after an unfinished time assignment.\nTime assignment ID: %0\nTime assignment ID: %1")
                                    .arg(timeAssignmentsIter->id)
                                    .arg(timeAssignment.id);
                            goto after;
                        }

                        ui->timeEditTime->setMinimumTime(timeAdd(m_lastTimeAssignmentStart, QTime(0, 1)));
                        ui->pushButtonStart->setText(tr("Switch"));
                        ui->pushButtonEnd->setEnabled(true);
                        goto after;
                    }
                    else
                    {
                        m_timeAssignmentTime = timeAdd(m_timeAssignmentTime, timeAssignment.timespan);
                    }
                }

                if(m_timeAssignmentTime > bookingTimespan)
                {
                    errorMessage = tr("Time assignment time longer than booking time! Time assignment: %0 Booking: %1")
                            .arg(m_timeAssignmentTime.toString("HH:mm:ss"))
                            .arg(bookingTimespan.toString("HH:mm:ss"));

                    auto label = new QLabel(errorMessage, ui->scrollAreaWidgetContents);
                    ui->verticalLayout2->addWidget(label);
                    label->setMinimumHeight(20);
                    label->setMaximumHeight(20);
                }

                ui->verticalLayout2->addWidget(new BookingStrip(endBooking.id, endBooking.time, endBooking.type, m_settings, ui->scrollAreaWidgetContents));

                if(m_timeAssignmentTime > bookingTimespan)
                    goto after;
            }
        }
    }

    after:
    if(errorMessage.isEmpty())
        m_workingTimeLabel->setText(tr("%0: %1").arg(tr("Assigned time")).arg(tr("%0h").arg(m_timeAssignmentTime.toString(QStringLiteral("HH:mm")))));
    else
    {
        auto label = new QLabel(tr("Strip rendering aborted due error."), ui->scrollAreaWidgetContents);
        ui->verticalLayout2->addWidget(label);
        label->setMinimumHeight(20);
        label->setMaximumHeight(20);
    }

    ui->verticalLayout2->addStretch(1);

    if(!errorMessage.isEmpty())
    {
        QMessageBox::warning(this, tr("Illegal state!"), tr("Your bookings and time assignments for this day are in an illegal state!") % "\n\n" % errorMessage);
        return;
    }

    ui->timeEditTime->setEnabled(true);
    ui->comboBoxProject->setEnabled(true);
    ui->comboBoxSubproject->setEnabled(true);
    ui->comboBoxWorkpackage->setEnabled(true);
    ui->comboBoxText->setEnabled(true);
    ui->pushButtonStart->setEnabled(true);
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

void MainWindow::clearStrips()
{
    QLayoutItem *item;
    while(item = ui->verticalLayout2->takeAt(0))
    {
        delete item->widget();
        delete item;
    }
}

QString MainWindow::buildProjectString(const QString &project)
{
    if(m_projects.contains(project))
        return m_projects.value(project) % " (" % project % ")";
    else
    {
        qWarning() << "could not find project" << project;
        return project;
    }
}
