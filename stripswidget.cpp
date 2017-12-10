#include "stripswidget.h"

#include <QVBoxLayout>
#include <QVector>
#include <QLabel>
#include <QMessageBox>
#include <QMap>
#include <QStringBuilder>
#include <QDebug>

#include "models/bookingsmodel.h"
#include "models/timeassignmentsmodel.h"
#include "replies/getbookingsreply.h"
#include "replies/gettimeassignmentsreply.h"
#include "timeutils.h"
#include "stripfactory.h"

StripsWidget::StripsWidget(ZeiterfassungApi &erfassung, int userId, StripFactory &stripFactory,
                           const QMap<QString, QString> &projects, QWidget *parent) :
    QWidget(parent),
    m_erfassung(erfassung),
    m_userId(userId),
    m_stripFactory(stripFactory),
    m_projects(projects),
    m_layout(new QVBoxLayout(this)),
    m_bookingsModel(new BookingsModel(this)),
    m_timeAssignmentsModel(new TimeAssignmentsModel(this)),
    m_refreshing(false),
    m_startEnabled(false),
    m_endEnabled(false),
    m_getBookingsReply(Q_NULLPTR),
    m_getTimeAssignmentsReply(Q_NULLPTR)
{
    setLayout(m_layout);
}

BookingsModel *StripsWidget::bookingsModel() const
{
    return m_bookingsModel;
}

TimeAssignmentsModel *StripsWidget::timeAssignmentsModel() const
{
    return m_timeAssignmentsModel;
}

const QDate &StripsWidget::date() const
{
    return m_date;
}

void StripsWidget::setDate(const QDate &date)
{
    m_date = date;
    refresh();
}

const QVector<ZeiterfassungApi::Booking> &StripsWidget::bookings() const
{
    return m_bookings;
}

const QVector<ZeiterfassungApi::TimeAssignment> &StripsWidget::timeAssignments() const
{
    return m_timeAssignments;
}

const QTime StripsWidget::timeAssignmentTime() const
{
    return m_timeAssignmentTime;
}

const QTime StripsWidget::lastTimeAssignmentStart() const
{
    return m_lastTimeAssignmentStart;
}

const QTime StripsWidget::minimumTime() const
{
    return m_minimumTime;
}

bool StripsWidget::refreshing() const
{
    return m_refreshing;
}

bool StripsWidget::startEnabled() const
{
    return m_startEnabled;
}

bool StripsWidget::endEnabled() const
{
    return m_endEnabled;
}

void StripsWidget::refresh()
{
    clearStrips();

    m_layout->addWidget(new QLabel(tr("Loading..."), this));

    refreshBookings();
    refreshTimeAssignments();
}

void StripsWidget::refreshBookings()
{
    if(!m_date.isValid())
    {
        qWarning() << "invalid date";
        return;
    }

    m_bookings.clear();

    m_getBookingsReply = m_erfassung.doGetBookings(m_userId, m_date, m_date);
    connect(m_getBookingsReply, &ZeiterfassungReply::finished, this, &StripsWidget::getBookingsFinished);

    if(!m_refreshing)
    {
        m_refreshing = true;
        Q_EMIT refreshingChanged();
    }
}

void StripsWidget::refreshTimeAssignments()
{
    if(!m_date.isValid())
    {
        qWarning() << "invalid date";
        return;
    }

    m_timeAssignments.clear();

    m_getTimeAssignmentsReply = m_erfassung.doGetTimeAssignments(m_userId, m_date, m_date);
    connect(m_getTimeAssignmentsReply, &ZeiterfassungReply::finished, this, &StripsWidget::getTimeAssignmentsFinished);

    if(!m_refreshing)
    {
        m_refreshing = true;
        Q_EMIT refreshingChanged();
    }
}

bool StripsWidget::createStrips()
{
    clearStrips();

    QTime timeAssignmentTime(0, 0);
    QTime lastTimeAssignmentStart;
    bool endEnabled = false;

    auto bookingsIter = m_bookings.constBegin();
    auto timeAssignmentsIter = m_timeAssignments.constBegin();

    auto bookingTimespan = QTime(0, 0);

    const ZeiterfassungApi::Booking *lastBooking = Q_NULLPTR;
    const ZeiterfassungApi::TimeAssignment *lastTimeAssignment = Q_NULLPTR;

    QString errorMessage;

    while(true)
    {
        if(bookingsIter == m_bookings.constEnd() &&
           timeAssignmentsIter == m_timeAssignments.constEnd())
        {
            goto after;
        }

        if(bookingsIter == m_bookings.constEnd())
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

        lastTimeAssignmentStart = startBooking.time;
        appendBookingStartStrip(startBooking.id, startBooking.time);

        if(timeAssignmentsIter == m_timeAssignments.constEnd())
        {
            errorMessage = tr("Missing time assignment!");
            goto after;
        }

        auto timeAssignment = *timeAssignmentsIter++;
        if(timeAssignment.time != timeAssignmentTime)
        {
            errorMessage = tr("Expected %0 but received %1 in time assignment.\nTime assignment ID: %2")
                    .arg(timeAssignmentTime.toString(QStringLiteral("HH:mm:ss")))
                    .arg(timeAssignment.time.toString(QStringLiteral("HH:mm:ss")))
                    .arg(timeAssignment.id);
            goto after;
        }

        lastTimeAssignment = &timeAssignment;

        appendTimeAssignmentStrip(timeAssignment.id, timeAssignment.timespan, buildProjectString(timeAssignment.project),
                                  timeAssignment.subproject, timeAssignment.workpackage, timeAssignment.text);

        if(timeAssignment.timespan == QTime(0, 0))
        {
            if(bookingsIter != m_bookings.constEnd())
            {
                errorMessage = tr("There is another booking after an unfinished time assignment.\nBooking ID: %0\nTime assignment ID: %1")
                        .arg(bookingsIter->id)
                        .arg(timeAssignment.id);
                goto after;
            }

            if(timeAssignmentsIter != m_timeAssignments.constEnd())
            {
                errorMessage = tr("There is another time assignment after an unfinished time assignment.\nTime assignment ID: %0\nTime assignment ID: %1")
                        .arg(timeAssignmentsIter->id)
                        .arg(timeAssignment.id);
                goto after;
            }

            endEnabled = true;
            goto after;
        }
        else
        {
            timeAssignmentTime = timeAdd(timeAssignmentTime, timeAssignment.timespan);
            lastTimeAssignmentStart = timeAdd(lastTimeAssignmentStart, timeAssignment.timespan);

            if(bookingsIter == m_bookings.constEnd())
            {
                while(true)
                {
                    if(timeAssignmentsIter == m_timeAssignments.constEnd())
                    {
                        errorMessage = tr("The last time assignment is finished without end booking\nTime assignment ID: %0")
                                .arg(timeAssignment.id);
                        goto after;
                    }

                    timeAssignment = *timeAssignmentsIter++;
                    if(timeAssignment.time != timeAssignmentTime)
                    {
                        errorMessage = tr("Expected %0 but received %1 in time assignment.\nTime assignment ID: %2")
                                .arg(timeAssignmentTime.toString(QStringLiteral("HH:mm:ss")))
                                .arg(timeAssignment.time.toString(QStringLiteral("HH:mm:ss")))
                                .arg(timeAssignment.id);
                        goto after;
                    }

                    lastTimeAssignment = &timeAssignment;

                    appendTimeAssignmentStrip(timeAssignment.id, timeAssignment.timespan, buildProjectString(timeAssignment.project),
                                              timeAssignment.subproject, timeAssignment.workpackage, timeAssignment.text);

                    if(timeAssignment.timespan == QTime(0, 0))
                    {
                        if(timeAssignmentsIter != m_timeAssignments.constEnd())
                        {
                            errorMessage = tr("There is another time assignment after an unfinished time assignment.\n"
                                              "Time assignment ID: %0\nTime assignment ID: %1")
                                    .arg(timeAssignment.id)
                                    .arg(timeAssignmentsIter->id);
                            goto after;
                        }

                        endEnabled = true;
                        goto after;
                    }
                    else
                    {
                        timeAssignmentTime = timeAdd(timeAssignmentTime, timeAssignment.timespan);
                        lastTimeAssignmentStart = timeAdd(lastTimeAssignmentStart, timeAssignment.timespan);
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

                while(timeAssignmentTime < bookingTimespan)
                {
                    if(timeAssignmentsIter == m_timeAssignments.constEnd())
                    {
                        errorMessage = tr("Missing time assignment! Missing: %0h")
                                .arg(timeBetween(timeAssignmentTime, bookingTimespan).toString(QStringLiteral("HH:mm:ss")));

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
                    if(timeAssignment.time != timeAssignmentTime)
                    {
                        errorMessage = tr("Expected %0 but received %1 in time assignment.\nTime assignment ID: %2")
                                .arg(timeAssignmentTime.toString(QStringLiteral("HH:mm:ss")))
                                .arg(timeAssignment.time.toString(QStringLiteral("HH:mm:ss")))
                                .arg(timeAssignment.id);
                        goto after;
                    }

                    lastTimeAssignment = &timeAssignment;

                    appendTimeAssignmentStrip(timeAssignment.id, timeAssignment.timespan, buildProjectString(timeAssignment.project),
                                              timeAssignment.subproject, timeAssignment.workpackage, timeAssignment.text);

                    if(timeAssignment.timespan == QTime(0, 0))
                    {
                        if(bookingsIter != m_bookings.constEnd())
                        {
                            errorMessage = tr("There is another booking after an unfinished time assignment.\n"
                                              "Booking ID: %0\nTime assignment ID: %1")
                                    .arg(bookingsIter->id)
                                    .arg(timeAssignment.id);
                            goto after;
                        }

                        if(timeAssignmentsIter != m_timeAssignments.constEnd())
                        {
                            errorMessage = tr("There is another time assignment after an unfinished time assignment.\nTime assignment ID: %0\nTime assignment ID: %1")
                                    .arg(timeAssignmentsIter->id)
                                    .arg(timeAssignment.id);
                            goto after;
                        }

                        endEnabled = true;
                        goto after;
                    }
                    else
                    {
                        timeAssignmentTime = timeAdd(timeAssignmentTime, timeAssignment.timespan);
                    }
                }

                if(timeAssignmentTime > bookingTimespan)
                {
                    errorMessage = tr("Time assignment time longer than booking time! Time assignment: %0 Booking: %1")
                            .arg(timeAssignmentTime.toString(QStringLiteral("HH:mm:ss")))
                            .arg(bookingTimespan.toString(QStringLiteral("HH:mm:ss")));

                    auto label = new QLabel(errorMessage, this);
                    m_layout->addWidget(label);
                    label->setMinimumHeight(20);
                    label->setMaximumHeight(20);
                }

                appendBookingEndStrip(endBooking.id, endBooking.time);

                if(timeAssignmentTime > bookingTimespan)
                    goto after;
            }
        }
    }

    after:
    auto startEnabled = !errorMessage.isEmpty();
    if(!errorMessage.isEmpty())
    {
        startEnabled = false;
        endEnabled = false;

        timeAssignmentTime = QTime();
        lastTimeAssignmentStart = QTime();

        auto label = new QLabel(tr("Strip rendering aborted due error."), this);
        m_layout->addWidget(label);
        label->setMinimumHeight(20);
        label->setMaximumHeight(20);

        QMessageBox::warning(this, tr("Illegal state!"),
                             tr("Your bookings and time assignments for this day are in an illegal state!") % "\n\n" %
                             m_date.toString("dd.MM.yyyy") % "\n\n" %
                             errorMessage);
    }

    if(m_startEnabled != startEnabled)
    {
        m_startEnabled = startEnabled;
        Q_EMIT startEnabledChanged();
    }

    if(m_endEnabled != endEnabled)
    {
        m_endEnabled = endEnabled;
        Q_EMIT endEnabledChanged();
    }

    if(m_timeAssignmentTime != timeAssignmentTime)
    {
        m_timeAssignmentTime = timeAssignmentTime;
        Q_EMIT timeAssignmentTimeChanged();
    }

    if(m_lastTimeAssignmentStart != lastTimeAssignmentStart)
    {
        m_lastTimeAssignmentStart = lastTimeAssignmentStart;
        Q_EMIT lastTimeAssignmentStartChanged();
    }

    return !errorMessage.isEmpty();
}

void StripsWidget::clearStrips()
{
    while(QLayoutItem *item = m_layout->takeAt(0))
    {
        delete item->widget();
        delete item;
    }
}

void StripsWidget::getBookingsFinished()
{
    m_bookings = m_getBookingsReply->bookings();

    if(!m_getTimeAssignmentsReply)
    {
        if(m_refreshing)
        {
            m_refreshing = false;
            Q_EMIT refreshingChanged();
        }

        createStrips();
    }

    m_getBookingsReply->deleteLater();
    m_getBookingsReply = Q_NULLPTR;
}

void StripsWidget::getTimeAssignmentsFinished()
{
    m_timeAssignments = m_getTimeAssignmentsReply->timeAssignments();

    if(!m_getBookingsReply)
    {
        if(m_refreshing)
        {
            m_refreshing = false;
            Q_EMIT refreshingChanged();
        }

        createStrips();
    }

    m_getTimeAssignmentsReply->deleteLater();
    m_getTimeAssignmentsReply = Q_NULLPTR;
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
    auto widget = m_stripFactory.createBookingStartStrip(this);

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
    auto widget = m_stripFactory.createBookingEndStrip(this);

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
    auto widget = m_stripFactory.createTimeAssignmentStrip(this);

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
