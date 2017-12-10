#include "stripswidget.h"

#include <QVBoxLayout>
#include <QVector>
#include <QLabel>
#include <QMessageBox>
#include <QMap>
#include <QStringBuilder>
#include <QDebug>

#include "timeutils.h"
#include "stripfactory.h"

StripsWidget::StripsWidget(StripFactory *stripFactory, const QMap<QString, QString> &projects,
                           QWidget *parent) :
    QWidget(parent),
    m_stripFactory(stripFactory),
    m_projects(projects),
    m_layout(new QVBoxLayout(this))
{
    setLayout(m_layout);
}

bool StripsWidget::createStrips(const QVector<ZeiterfassungApi::Booking> &bookings,
                                const QVector<ZeiterfassungApi::TimeAssignment> &timeAssignments)
{
    auto bookingsIter = bookings.constBegin();
    auto timeAssignmentsIter = timeAssignments.constBegin();

    m_timeAssignmentTime = QTime(0, 0);
    auto bookingTimespan = QTime(0, 0);

    const ZeiterfassungApi::Booking *lastBooking = Q_NULLPTR;
    const ZeiterfassungApi::TimeAssignment *lastTimeAssignment = Q_NULLPTR;

    QString errorMessage;

    while(true)
    {
        if(bookingsIter == bookings.constEnd() &&
           timeAssignmentsIter == timeAssignments.constEnd())
        {
            goto after;
        }

        if(bookingsIter == bookings.constEnd())
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
                                    this);
            m_layout->addWidget(label);
            label->setMinimumHeight(20);
            label->setMaximumHeight(20);
        }

        lastBooking = &startBooking;

        m_lastTimeAssignmentStart = startBooking.time;
        appendBookingStartStrip(startBooking.id, startBooking.time);

        if(timeAssignmentsIter == timeAssignments.constEnd())
        {
            errorMessage = tr("Missing time assignment!");
            goto after;
        }

        auto timeAssignment = *timeAssignmentsIter++;
        if(timeAssignment.time != m_timeAssignmentTime)
        {
            errorMessage = tr("Expected %0 but received %1 in time assignment.\nTime assignment ID: %2")
                    .arg(m_timeAssignmentTime.toString(QStringLiteral("HH:mm:ss")))
                    .arg(timeAssignment.time.toString(QStringLiteral("HH:mm:ss")))
                    .arg(timeAssignment.id);
            goto after;
        }

        lastTimeAssignment = &timeAssignment;

        appendTimeAssignmentStrip(timeAssignment.id, timeAssignment.timespan, buildProjectString(timeAssignment.project),
                                  timeAssignment.subproject, timeAssignment.workpackage, timeAssignment.text);

        if(timeAssignment.timespan == QTime(0, 0))
        {
            if(bookingsIter != bookings.constEnd())
            {
                errorMessage = tr("There is another booking after an unfinished time assignment.\nBooking ID: %0\nTime assignment ID: %1")
                        .arg(bookingsIter->id)
                        .arg(timeAssignment.id);
                goto after;
            }

            if(timeAssignmentsIter != timeAssignments.constEnd())
            {
                errorMessage = tr("There is another time assignment after an unfinished time assignment.\nTime assignment ID: %0\nTime assignment ID: %1")
                        .arg(timeAssignmentsIter->id)
                        .arg(timeAssignment.id);
                goto after;
            }

            //ui->timeEditTime->setMinimumTime(timeAdd(m_lastTimeAssignmentStart, QTime(0, 1)));
            //ui->pushButtonStart->setText(tr("Switch"));
            //ui->pushButtonEnd->setEnabled(true);
            goto after;
        }
        else
        {
            m_timeAssignmentTime = timeAdd(m_timeAssignmentTime, timeAssignment.timespan);
            m_lastTimeAssignmentStart = timeAdd(m_lastTimeAssignmentStart, timeAssignment.timespan);

            if(bookingsIter == bookings.constEnd())
            {
                while(true)
                {
                    if(timeAssignmentsIter == timeAssignments.constEnd())
                    {
                        errorMessage = tr("The last time assignment is finished without end booking\nTime assignment ID: %0")
                                .arg(timeAssignment.id);
                        goto after;
                    }

                    timeAssignment = *timeAssignmentsIter++;
                    if(timeAssignment.time != m_timeAssignmentTime)
                    {
                        errorMessage = tr("Expected %0 but received %1 in time assignment.\nTime assignment ID: %2")
                                .arg(m_timeAssignmentTime.toString(QStringLiteral("HH:mm:ss")))
                                .arg(timeAssignment.time.toString(QStringLiteral("HH:mm:ss")))
                                .arg(timeAssignment.id);
                        goto after;
                    }

                    lastTimeAssignment = &timeAssignment;

                    appendTimeAssignmentStrip(timeAssignment.id, timeAssignment.timespan, buildProjectString(timeAssignment.project),
                                              timeAssignment.subproject, timeAssignment.workpackage, timeAssignment.text);

                    if(timeAssignment.timespan == QTime(0, 0))
                    {
                        if(timeAssignmentsIter != timeAssignments.constEnd())
                        {
                            errorMessage = tr("There is another time assignment after an unfinished time assignment.\n"
                                              "Time assignment ID: %0\nTime assignment ID: %1")
                                    .arg(timeAssignment.id)
                                    .arg(timeAssignmentsIter->id);
                            goto after;
                        }

                        //ui->timeEditTime->setMinimumTime(timeAdd(m_lastTimeAssignmentStart, QTime(0, 1)));
                        //ui->pushButtonStart->setText(tr("Switch"));
                        //ui->pushButtonEnd->setEnabled(true);
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
                //ui->timeEditTime->setMinimumTime(timeAdd(endBooking.time, QTime(0, 1)));

                while(m_timeAssignmentTime < bookingTimespan)
                {
                    if(timeAssignmentsIter == timeAssignments.constEnd())
                    {
                        errorMessage = tr("Missing time assignment! Missing: %0h")
                                .arg(timeBetween(m_timeAssignmentTime, bookingTimespan).toString(QStringLiteral("HH:mm:ss")));

                        {
                            auto label = new QLabel(errorMessage, this);
                            m_layout->addWidget(label);
                            label->setMinimumHeight(20);
                            label->setMaximumHeight(20);
                        }

                        appendBookingEndStrip(endBooking.id, endBooking.time);

                        goto after;
                    }

                    timeAssignment = *timeAssignmentsIter++;
                    if(timeAssignment.time != m_timeAssignmentTime)
                    {
                        errorMessage = tr("Expected %0 but received %1 in time assignment.\nTime assignment ID: %2")
                                .arg(m_timeAssignmentTime.toString(QStringLiteral("HH:mm:ss")))
                                .arg(timeAssignment.time.toString(QStringLiteral("HH:mm:ss")))
                                .arg(timeAssignment.id);
                        goto after;
                    }

                    lastTimeAssignment = &timeAssignment;

                    appendTimeAssignmentStrip(timeAssignment.id, timeAssignment.timespan, buildProjectString(timeAssignment.project),
                                              timeAssignment.subproject, timeAssignment.workpackage, timeAssignment.text);

                    if(timeAssignment.timespan == QTime(0, 0))
                    {
                        if(bookingsIter != bookings.constEnd())
                        {
                            errorMessage = tr("There is another booking after an unfinished time assignment.\n"
                                              "Booking ID: %0\nTime assignment ID: %1")
                                    .arg(bookingsIter->id)
                                    .arg(timeAssignment.id);
                            goto after;
                        }

                        if(timeAssignmentsIter != timeAssignments.constEnd())
                        {
                            errorMessage = tr("There is another time assignment after an unfinished time assignment.\nTime assignment ID: %0\nTime assignment ID: %1")
                                    .arg(timeAssignmentsIter->id)
                                    .arg(timeAssignment.id);
                            goto after;
                        }

                        //ui->timeEditTime->setMinimumTime(timeAdd(m_lastTimeAssignmentStart, QTime(0, 1)));
                        //ui->pushButtonStart->setText(tr("Switch"));
                        //ui->pushButtonEnd->setEnabled(true);
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
                            .arg(m_timeAssignmentTime.toString(QStringLiteral("HH:mm:ss")))
                            .arg(bookingTimespan.toString(QStringLiteral("HH:mm:ss")));

                    auto label = new QLabel(errorMessage, this);
                    m_layout->addWidget(label);
                    label->setMinimumHeight(20);
                    label->setMaximumHeight(20);
                }

                appendBookingEndStrip(endBooking.id, endBooking.time);

                if(m_timeAssignmentTime > bookingTimespan)
                    goto after;
            }
        }
    }

    after:
    if(!errorMessage.isEmpty())
    {
        auto label = new QLabel(tr("Strip rendering aborted due error."), this);
        m_layout->addWidget(label);
        label->setMinimumHeight(20);
        label->setMaximumHeight(20);

        QMessageBox::warning(this, tr("Illegal state!"),
                             tr("Your bookings and time assignments for this day are in an illegal state!") % "\n\n" % errorMessage);
        return false;
    }

    return true;
}

void StripsWidget::clearStrips()
{
    while(QLayoutItem *item = m_layout->takeAt(0))
    {
        delete item->widget();
        delete item;
    }
}

const QTime StripsWidget::timeAssignmentTime() const
{
    return m_timeAssignmentTime;
}

const QTime StripsWidget::lastTimeAssignmentStart() const
{
    return m_lastTimeAssignmentStart;
}

QString StripsWidget::buildProjectString(const QString &project)
{
    if(m_projects.contains(project))
        return m_projects.value(project) % " (" % project % ")";
    else
    {
        qWarning() << "could not find project" << project;
        return project;
    }
}

QWidget *StripsWidget::appendBookingStartStrip(int id, const QTime &time)
{
    auto widget = m_stripFactory->createBookingStartStrip(this);

    if(auto labelTime = widget->findChild<QWidget*>("labelTime"))
        labelTime->setProperty("text", time.toString(QStringLiteral("HH:mm")));
    else
        qWarning() << "no labelTime found!";

    if(auto labelId = widget->findChild<QWidget*>("labelId"))
        labelId->setProperty("text", QString::number(id));
    else
        qWarning() << "no labelId found!";

    m_layout->addWidget(widget);

    return widget;
}

QWidget *StripsWidget::appendBookingEndStrip(int id, const QTime &time)
{
    auto widget = m_stripFactory->createBookingEndStrip(this);

    if(auto labelTime = widget->findChild<QWidget*>("labelTime"))
        labelTime->setProperty("text", time.toString(QStringLiteral("HH:mm")));
    else
        qWarning() << "no labelTime found!";

    if(auto labelId = widget->findChild<QWidget*>("labelId"))
        labelId->setProperty("text", QString::number(id));
    else
        qWarning() << "no labelId found!";

    m_layout->addWidget(widget);

    return widget;
}

QWidget *StripsWidget::appendTimeAssignmentStrip(int id, const QTime &duration, const QString &project, const QString &subproject, const QString &workpackage, const QString &text)
{
    auto widget = m_stripFactory->createTimeAssignmentStrip(this);

    if(auto labelTime = widget->findChild<QWidget*>("labelTime"))
        labelTime->setProperty("text", duration == QTime(0, 0) ? tr("Open") : duration.toString(QStringLiteral("HH:mm")));
    else
        qWarning() << "no labelTime found!";

    if(auto labelProject = widget->findChild<QWidget*>("labelProject"))
        labelProject->setProperty("text", project);
    else
        qWarning() << "no labelProject found!";

    if(auto labelId = widget->findChild<QWidget*>("labelId"))
        labelId->setProperty("text", QString::number(id));
    else
        qWarning() << "no labelId found!";

    if(auto labelSubproject = widget->findChild<QWidget*>("labelSubproject"))
        labelSubproject->setProperty("text", subproject);
    else
        qWarning() << "no labelSubproject found!";

    if(auto labelWorkpackage = widget->findChild<QWidget*>("labelWorkpackage"))
        labelWorkpackage->setProperty("text", workpackage);
    else
        qWarning() << "no labelWorkpackage found!";

    if(auto labelText = widget->findChild<QWidget*>("labelText"))
        labelText->setProperty("text", text);
    else
        qWarning() << "no labelText found!";

    m_layout->addWidget(widget);

    return widget;
}
