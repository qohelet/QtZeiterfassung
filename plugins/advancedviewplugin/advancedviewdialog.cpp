#include "advancedviewdialog.h"
#include "ui_advancedviewdialog.h"

#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QStringBuilder>

#include "replies/createbookingreply.h"
#include "replies/updatebookingreply.h"
#include "replies/deletebookingreply.h"
#include "replies/createtimeassignmentreply.h"
#include "replies/updatetimeassignmentreply.h"
#include "replies/deletetimeassignmentreply.h"

#include "stripswidget.h"
#include "mainwindow.h"
#include "timeutils.h"
#include "dialogs/bookingdialog.h"
#include "dialogs/timeassignmentdialog.h"
#include "models/bookingsmodel.h"
#include "models/timeassignmentsmodel.h"

AdvancedViewDialog::AdvancedViewDialog(StripsWidget &stripsWidget) :
    QDialog(&stripsWidget.mainWindow()),
    ui(new Ui::AdvancedViewDialog),
    m_stripsWidget(stripsWidget),
    m_bookingsModel(new BookingsModel(stripsWidget, this)),
    m_timeAssignmentsModel(new TimeAssignmentsModel(stripsWidget, this))
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    ui->bookingsView->setModel(m_bookingsModel);
    ui->bookingsView->setEnabled(m_bookingsModel->enabled());
    connect(m_bookingsModel, &BookingsModel::enabledChanged, ui->bookingsView, &QWidget::setEnabled);
    connect(ui->bookingsView, &QWidget::customContextMenuRequested, this, &AdvancedViewDialog::contextMenuBooking);

    ui->timeAssignmentsView->setModel(m_timeAssignmentsModel);
    ui->timeAssignmentsView->setEnabled(m_timeAssignmentsModel->enabled());
    connect(m_timeAssignmentsModel, &TimeAssignmentsModel::enabledChanged, ui->timeAssignmentsView, &QWidget::setEnabled);
    connect(ui->timeAssignmentsView, &QWidget::customContextMenuRequested, this, &AdvancedViewDialog::contextMenuTimeAssignment);
}

AdvancedViewDialog::~AdvancedViewDialog()
{
    delete ui;
}

void AdvancedViewDialog::contextMenuBooking(const QPoint &pos)
{
    auto index = ui->bookingsView->indexAt(pos);

    if(!index.isValid())
    {
        QMenu menu;
        auto createAction = menu.addAction(tr("Create booking"));
        auto refreshAction = menu.addAction(QIcon(QPixmap(QStringLiteral(":/zeiterfassungguilib/images/refresh.png"))), tr("Refresh bookings"));
        auto selectedAction = menu.exec(ui->bookingsView->viewport()->mapToGlobal(pos));
        if(selectedAction == createAction)
        {
            BookingDialog dialog(this);
            dialog.setTime(timeNormalise(QTime::currentTime()));
            again2:
            if(dialog.exec() == QDialog::Accepted)
            {
                auto reply = m_stripsWidget.mainWindow().erfassung().doCreateBooking(
                    m_stripsWidget.mainWindow().userInfo().userId,
                    m_stripsWidget.date(),
                    dialog.getTime(),
                    dialog.getTimespan(),
                    dialog.getType(),
                    dialog.getText()
                );

                reply->waitForFinished();

                if(reply->success())
                {
                    m_stripsWidget.refreshBookings();
                }
                else
                {
                    QMessageBox::warning(this, tr("Could not create booking!"), tr("Could not create booking!") % "\n\n" % reply->message());
                    goto again2;
                }
            }
        }
        else if(selectedAction == refreshAction)
        {
            m_stripsWidget.refreshBookings();
        }
    }
    else
    {
        auto booking = m_stripsWidget.bookings().at(index.row());

        QMenu menu;
        auto editAction = menu.addAction(tr("Edit booking"));
        auto deleteAction = menu.addAction(tr("Delete booking"));
        auto selectedAction = menu.exec(ui->bookingsView->viewport()->mapToGlobal(pos));
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
                auto reply = m_stripsWidget.mainWindow().erfassung().doUpdateBooking(
                    booking.id,
                    m_stripsWidget.mainWindow().userInfo().userId,
                    m_stripsWidget.date(),
                    dialog.getTime(),
                    dialog.getTimespan(),
                    dialog.getType(),
                    dialog.getText()
                );

                reply->waitForFinished();

                if(reply->success())
                {
                    m_stripsWidget.refreshBookings();
                }
                else
                {
                    QMessageBox::warning(this, tr("Could not edit booking!"), tr("Could not edit booking!") % "\n\n" % reply->message());
                    goto again1;
                }
            }
        }
        else if(selectedAction == deleteAction)
        {
            QMessageBox msgBox;
            msgBox.setText(tr("Do you really want to delete the booking?"));
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            if(msgBox.exec() == QMessageBox::Yes)
            {
                auto reply = m_stripsWidget.mainWindow().erfassung().doDeleteBooking(booking.id);

                reply->waitForFinished();

                if(reply->success())
                    m_stripsWidget.refreshBookings();
                else
                    QMessageBox::warning(this, tr("Could not delete booking!"), tr("Could not delete booking!") % "\n\n" % reply->message());
            }
        }
    }
}

void AdvancedViewDialog::contextMenuTimeAssignment(const QPoint &pos)
{
    auto index = ui->timeAssignmentsView->indexAt(pos);

    if(!index.isValid())
    {
        QMenu menu;
        auto createAction = menu.addAction(tr("Create time assignment"));
        auto refreshAction = menu.addAction(QIcon(QPixmap(QStringLiteral(":/zeiterfassungguilib/images/refresh.png"))), tr("Refresh time assignments"));
        auto selectedAction = menu.exec(ui->timeAssignmentsView->viewport()->mapToGlobal(pos));
        if(selectedAction == createAction)
        {
            TimeAssignmentDialog dialog(m_stripsWidget.mainWindow().projects(),
                                        m_stripsWidget.mainWindow().settings(), this);
            again2:
            if(dialog.exec() == QDialog::Accepted)
            {
                auto reply = m_stripsWidget.mainWindow().erfassung().doCreateTimeAssignment(
                    m_stripsWidget.mainWindow().userInfo().userId,
                    m_stripsWidget.date(),
                    dialog.getTime(),
                    dialog.getTimespan(),
                    dialog.getProject(),
                    dialog.getSubproject(),
                    dialog.getWorkpackage(),
                    dialog.getText()
                );

                reply->waitForFinished();

                if(reply->success())
                {
                    m_stripsWidget.refreshTimeAssignments();
                }
                else
                {
                    QMessageBox::warning(this, tr("Could not create time assignment!"), tr("Could not create time assignment!") % "\n\n" % reply->message());
                    goto again2;
                }
            }
        }
        else if(selectedAction == refreshAction)
        {
            m_stripsWidget.refreshTimeAssignments();
        }
    }
    else
    {
        auto timeAssignment = m_stripsWidget.timeAssignments().at(index.row());

        QMenu menu;
        auto editAction = menu.addAction(tr("Edit time assignment"));
        auto deleteAction = menu.addAction(tr("Delete time assignment"));
        auto selectedAction = menu.exec(ui->timeAssignmentsView->viewport()->mapToGlobal(pos));
        if(selectedAction == editAction)
        {
            TimeAssignmentDialog dialog(m_stripsWidget.mainWindow().projects(),
                                        m_stripsWidget.mainWindow().settings(), this);
            dialog.setTime(timeAssignment.time);
            dialog.setTimespan(timeAssignment.timespan);
            dialog.setProject(timeAssignment.project);
            dialog.setSubproject(timeAssignment.subproject);
            dialog.setWorkpackage(timeAssignment.workpackage);
            dialog.setText(timeAssignment.text);
            again1:
            if(dialog.exec() == QDialog::Accepted)
            {
                auto reply = m_stripsWidget.mainWindow().erfassung().doUpdateTimeAssignment(
                    timeAssignment.id,
                    m_stripsWidget.mainWindow().userInfo().userId,
                    m_stripsWidget.date(),
                    dialog.getTime(),
                    dialog.getTimespan(),
                    dialog.getProject(),
                    dialog.getSubproject(),
                    dialog.getWorkpackage(),
                    dialog.getText()
                );

                reply->waitForFinished();

                if(reply->success())
                {
                    m_stripsWidget.refreshTimeAssignments();
                }
                else
                {
                    QMessageBox::warning(this, tr("Could not edit time assignment!"), tr("Could not edit time assignment!") % "\n\n" % reply->message());
                    goto again1;
                }
            }
        }
        else if(selectedAction == deleteAction)
        {
            QMessageBox msgBox;
            msgBox.setText(tr("Do you really want to delete the time assignment?"));
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            if(msgBox.exec() == QMessageBox::Yes)
            {
                auto reply = m_stripsWidget.mainWindow().erfassung().doDeleteTimeAssignment(timeAssignment.id);

                reply->waitForFinished();

                if(reply->success())
                {
                    m_stripsWidget.refreshTimeAssignments();
                }
                else
                    QMessageBox::warning(this, tr("Could not delete time assignment!"), tr("Could not delete time assignment!") % "\n\n" % reply->message());
            }
        }
    }
}
