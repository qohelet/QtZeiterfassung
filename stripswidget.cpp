#include "stripswidget.h"

#include <QVBoxLayout>
#include <QVector>
#include <QLabel>
#include <QMessageBox>
#include <QMap>
#include <QStringBuilder>
#include <QDebug>

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
    m_refreshing(false),
    m_refreshingBookings(false),
    m_refreshingTimeAssignments(false),
    m_startEnabled(false),
    m_endEnabled(false),
    m_getBookingsReply(Q_NULLPTR),
    m_getTimeAssignmentsReply(Q_NULLPTR)
{
    setLayout(m_layout);
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

const QTime &StripsWidget::timeAssignmentTime() const
{
    return m_timeAssignmentTime;
}

const QTime &StripsWidget::lastTimeAssignmentStart() const
{
    return m_lastTimeAssignmentStart;
}

const QTime &StripsWidget::minimumTime() const
{
    return m_minimumTime;
}

bool StripsWidget::refreshing() const
{
    return m_refreshing;
}

bool StripsWidget::refreshingBookings() const
{
    return m_refreshingBookings;
}

bool StripsWidget::refreshingTimeAssignments() const
{
    return m_refreshingTimeAssignments;
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
    m_layout->addStretch(1);

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

    if(m_bookings.count())
    {
        m_bookings.clear();
        Q_EMIT bookingsChanged(m_bookings);
    }

    if(!m_refreshing)
        Q_EMIT refreshingChanged(m_refreshing = true);

    if(!m_refreshingBookings)
        Q_EMIT refreshingBookingsChanged(m_refreshingBookings = true);

    invalidateValues();

    m_getBookingsReply = m_erfassung.doGetBookings(m_userId, m_date, m_date);
    connect(m_getBookingsReply, &ZeiterfassungReply::finished, this, &StripsWidget::getBookingsFinished);
}

void StripsWidget::refreshTimeAssignments()
{
    if(!m_date.isValid())
    {
        qWarning() << "invalid date";
        return;
    }

    if(m_timeAssignments.count())
    {
        m_timeAssignments.clear();
        Q_EMIT timeAssignmentsChanged(m_timeAssignments);
    }

    if(!m_refreshing)
        Q_EMIT refreshingChanged(m_refreshing = true);

    if(!m_refreshingTimeAssignments)
        Q_EMIT refreshingTimeAssignmentsChanged(m_refreshingTimeAssignments = true);

    invalidateValues();

    m_getTimeAssignmentsReply = m_erfassung.doGetTimeAssignments(m_userId, m_date, m_date);
    connect(m_getTimeAssignmentsReply, &ZeiterfassungReply::finished, this, &StripsWidget::getTimeAssignmentsFinished);
}

bool StripsWidget::createStrips()
{
    clearStrips();

    QTime timeAssignmentTime(0, 0);
    QTime lastTimeAssignmentStart;
    QTime minimumTime(0, 0);
    bool endEnabled = false;

    auto bookingsIter = m_bookings.constBegin();
    auto timeAssignmentsIter = m_timeAssignments.constBegin();

    auto bookingTimespan = QTime(0, 0);

    const ZeiterfassungApi::Booking *lastBooking = Q_NULLPTR;

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
            auto breakTime = timeBetween(lastBooking->time, startBooking.time);
            auto label = new QLabel(tr("%0: %1").arg(tr("Break")).arg(tr("%0h").arg(breakTime.toString(QStringLiteral("HH:mm")))), this);
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

            minimumTime = timeAdd(lastTimeAssignmentStart, QTime(0, 1));
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

                        minimumTime = timeAdd(lastTimeAssignmentStart, QTime(0, 1));
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
                minimumTime = timeAdd(endBooking.time, QTime(0, 1));

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

                        minimumTime = timeAdd(lastTimeAssignmentStart, QTime(0, 1));
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
    auto startEnabled = errorMessage.isEmpty();

    if(errorMessage.isEmpty())
    {
        auto label = new QLabel(tr("%0: %1")
                                .arg(tr("Assigned time"))
                                .arg(tr("%0h").arg(timeAssignmentTime.toString(QStringLiteral("HH:mm")))), this);
        m_layout->addWidget(label);
        label->setMinimumHeight(20);
        label->setMaximumHeight(20);
    }
    else
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
                             m_date.toString(QStringLiteral("dd.MM.yyyy")) % "\n\n" %
                             errorMessage);
    }

    if(m_timeAssignmentTime != timeAssignmentTime)
        Q_EMIT timeAssignmentTimeChanged(m_timeAssignmentTime = timeAssignmentTime);

    if(m_lastTimeAssignmentStart != lastTimeAssignmentStart)
        Q_EMIT lastTimeAssignmentStartChanged(m_lastTimeAssignmentStart = lastTimeAssignmentStart);

    if(m_minimumTime != minimumTime)
        Q_EMIT minimumTimeChanged(m_minimumTime = minimumTime);

    if(m_startEnabled != startEnabled)
        Q_EMIT startEnabledChanged(m_startEnabled = startEnabled);

    if(m_endEnabled != endEnabled)
        Q_EMIT endEnabledChanged(m_endEnabled = endEnabled);

    m_layout->addStretch(1);

    return !errorMessage.isEmpty();
}

void StripsWidget::clearStrips()
{
    while(QLayoutItem *item = m_layout->takeAt(0))
    {
        delete item->widget();
        delete item;
    }

    auto label = new QLabel(this);
    if(m_date.isValid())
        label->setText(tr("%0 (%1)")
            .arg(std::array<QString, 7> { tr("Monday"), tr("Tuesday"), tr("Wednesday"), tr("Thursday"),
                                          tr("Friday"), tr("Saturday"), tr("Sunday") }[m_date.dayOfWeek() - 1])
            .arg(m_date.toString(tr("dd.MM"))));
    else
        label->setText(tr("Invalid"));
    {
        auto font = label->font();
        font.setBold(true);
        label->setFont(font);
    }
    m_layout->addWidget(label);
}

void StripsWidget::getBookingsFinished()
{
    Q_EMIT bookingsChanged(m_bookings = m_getBookingsReply->bookings());

    if(m_refreshingBookings)
        Q_EMIT refreshingBookingsChanged(m_refreshingBookings = false);

    if(!m_getTimeAssignmentsReply)
    {
        if(m_refreshing)
            Q_EMIT refreshingChanged(m_refreshing = false);

        createStrips();
    }

    m_getBookingsReply->deleteLater();
    m_getBookingsReply = Q_NULLPTR;
}

void StripsWidget::getTimeAssignmentsFinished()
{
    Q_EMIT timeAssignmentsChanged(m_timeAssignments = m_getTimeAssignmentsReply->timeAssignments());

    if(m_refreshingTimeAssignments)
        Q_EMIT refreshingTimeAssignmentsChanged(m_refreshingTimeAssignments = false);

    if(!m_getBookingsReply)
    {
        if(m_refreshing)
            Q_EMIT refreshingChanged(m_refreshing = false);

        createStrips();
    }

    m_getTimeAssignmentsReply->deleteLater();
    m_getTimeAssignmentsReply = Q_NULLPTR;
}

void StripsWidget::invalidateValues()
{
    if(m_timeAssignmentTime.isValid())
        Q_EMIT timeAssignmentTimeChanged(m_timeAssignmentTime = QTime());

    if(m_lastTimeAssignmentStart.isValid())
        Q_EMIT lastTimeAssignmentStartChanged(m_lastTimeAssignmentStart = QTime());

    if(m_minimumTime.isValid())
        Q_EMIT minimumTimeChanged(m_minimumTime = QTime());

    if(m_startEnabled)
        Q_EMIT startEnabledChanged(m_startEnabled = false);

    if(m_endEnabled)
        Q_EMIT endEnabledChanged(m_endEnabled = false);
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

    if(auto labelTime = widget->findChild<QWidget*>(QStringLiteral("labelTime")))
        labelTime->setProperty("text", time.toString(QStringLiteral("HH:mm")));
    else
        qWarning() << "no labelTime found!";

    if(auto labelId = widget->findChild<QWidget*>(QStringLiteral("labelId")))
        labelId->setProperty("text", QString::number(id));
    else
        qWarning() << "no labelId found!";

    m_layout->addWidget(widget);

    return widget;
}

QWidget *StripsWidget::appendBookingEndStrip(int id, const QTime &time)
{
    auto widget = m_stripFactory.createBookingEndStrip(this);

    if(auto labelTime = widget->findChild<QWidget*>(QStringLiteral("labelTime")))
        labelTime->setProperty("text", time.toString(QStringLiteral("HH:mm")));
    else
        qWarning() << "no labelTime found!";

    if(auto labelId = widget->findChild<QWidget*>(QStringLiteral("labelId")))
        labelId->setProperty("text", QString::number(id));
    else
        qWarning() << "no labelId found!";

    m_layout->addWidget(widget);

    return widget;
}

QWidget *StripsWidget::appendTimeAssignmentStrip(int id, const QTime &duration, const QString &project, const QString &subproject, const QString &workpackage, const QString &text)
{
    auto widget = m_stripFactory.createTimeAssignmentStrip(this);

    if(auto labelTime = widget->findChild<QWidget*>(QStringLiteral("labelTime")))
        labelTime->setProperty("text", duration == QTime(0, 0) ? tr("Open") : duration.toString(QStringLiteral("HH:mm")));
    else
        qWarning() << "no labelTime found!";

    if(auto labelProject = widget->findChild<QWidget*>(QStringLiteral("labelProject")))
        labelProject->setProperty("text", project);
    else
        qWarning() << "no labelProject found!";

    if(auto labelId = widget->findChild<QWidget*>(QStringLiteral("labelId")))
        labelId->setProperty("text", QString::number(id));
    else
        qWarning() << "no labelId found!";

    if(auto labelSubproject = widget->findChild<QWidget*>(QStringLiteral("labelSubproject")))
        labelSubproject->setProperty("text", subproject);
    else
        qWarning() << "no labelSubproject found!";

    if(auto labelWorkpackage = widget->findChild<QWidget*>(QStringLiteral("labelWorkpackage")))
        labelWorkpackage->setProperty("text", workpackage);
    else
        qWarning() << "no labelWorkpackage found!";

    if(auto labelText = widget->findChild<QWidget*>(QStringLiteral("labelText")))
        labelText->setProperty("text", text);
    else
        qWarning() << "no labelText found!";

    m_layout->addWidget(widget);

    return widget;
}
