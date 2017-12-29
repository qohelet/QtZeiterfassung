#include "stripswidget.h"

#include <QVBoxLayout>
#include <QVector>
#include <QLabel>
#include <QMap>
#include <QStringBuilder>
#include <QDebug>

#include "mainwindow.h"
#include "zeiterfassungapi.h"
#include "timeutils.h"
#include "stripfactory.h"

StripsWidget::StripsWidget(MainWindow &mainWindow, QWidget *parent) :
    QWidget(parent),
    m_mainWindow(mainWindow),
    m_refreshing(false),
    m_refreshingBookings(false),
    m_refreshingTimeAssignments(false),
    m_startEnabled(false),
    m_endEnabled(false)
{
    auto layout = new QVBoxLayout(this);

    m_headerLayout = new QHBoxLayout;
    m_label = new QLabel;
    {
        auto font = m_label->font();
        font.setBold(true);
        m_label->setFont(font);
    }
    m_headerLayout->addWidget(m_label, 1);
    layout->addLayout(m_headerLayout);

    m_stripsLayout = new QVBoxLayout;
    layout->addLayout(m_stripsLayout);

    layout->addStretch(1);

    setLayout(layout);
}

MainWindow &StripsWidget::mainWindow() const
{
    return m_mainWindow;
}

QBoxLayout *StripsWidget::headerLayout() const
{
    return m_headerLayout;
}

QBoxLayout *StripsWidget::stripsLayout() const
{
    return m_stripsLayout;
}

QLabel *StripsWidget::label() const
{
    return m_label;
}

const QDate &StripsWidget::date() const
{
    return m_date;
}

void StripsWidget::setDate(const QDate &date)
{
    if(m_date != date)
    {
        Q_EMIT dateChanged(m_date = date);

        if(m_date.isValid())
            m_label->setText(tr("%0 (%1)")
                             .arg(std::array<QString, 7> { tr("Monday"), tr("Tuesday"), tr("Wednesday"), tr("Thursday"),
                                                           tr("Friday"), tr("Saturday"), tr("Sunday") }[m_date.dayOfWeek() - 1])
                             .arg(m_date.toString(tr("dd.MM.yyyy"))));
        else
            m_label->setText(tr("Invalid"));

        refresh();
    }
}

const QVector<GetBookingsReply::Booking> &StripsWidget::bookings() const
{
    return m_bookings;
}

const QVector<GetTimeAssignmentsReply::TimeAssignment> &StripsWidget::timeAssignments() const
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

    m_stripsLayout->addWidget(new QLabel(tr("Loading..."), this));

    refreshBookings(false);
    refreshTimeAssignments(false);
}

void StripsWidget::refreshBookings(bool createLabel)
{
    if(!m_date.isValid())
    {
        qWarning() << "invalid date";
        return;
    }

    if(createLabel)
    {
        clearStrips();

        m_stripsLayout->addWidget(new QLabel(tr("Loading..."), this));
    }

    if(!m_bookings.empty())
    {
        m_bookings.clear();
        Q_EMIT bookingsChanged(m_bookings);
    }

    if(!m_refreshing)
        Q_EMIT refreshingChanged(m_refreshing = true);

    if(!m_refreshingBookings)
        Q_EMIT refreshingBookingsChanged(m_refreshingBookings = true);

    invalidateValues();

    m_getBookingsReply = m_mainWindow.erfassung().doGetBookings(m_mainWindow.userInfo().userId, m_date, m_date);
    connect(m_getBookingsReply.get(), &ZeiterfassungReply::finished, this, &StripsWidget::getBookingsFinished);
}

void StripsWidget::refreshTimeAssignments(bool createLabel)
{
    if(!m_date.isValid())
    {
        qWarning() << "invalid date";
        return;
    }

    if(createLabel)
    {
        clearStrips();

        m_stripsLayout->addWidget(new QLabel(tr("Loading..."), this));
    }

    if(!m_timeAssignments.empty())
    {
        m_timeAssignments.clear();
        Q_EMIT timeAssignmentsChanged(m_timeAssignments);
    }

    if(!m_refreshing)
        Q_EMIT refreshingChanged(m_refreshing = true);

    if(!m_refreshingTimeAssignments)
        Q_EMIT refreshingTimeAssignmentsChanged(m_refreshingTimeAssignments = true);

    invalidateValues();

    m_getTimeAssignmentsReply = m_mainWindow.erfassung().doGetTimeAssignments(m_mainWindow.userInfo().userId, m_date, m_date);
    connect(m_getTimeAssignmentsReply.get(), &ZeiterfassungReply::finished, this, &StripsWidget::getTimeAssignmentsFinished);
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

    const GetBookingsReply::Booking *lastBooking = Q_NULLPTR;

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
            auto label = new QLabel(tr("%0: %1").arg(tr("Break")).arg(tr("%0h").arg(breakTime.toString(tr("HH:mm")))), this);
            m_stripsLayout->addWidget(label);
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

                auto currBookingDuration = timeBetween(startBooking.time, endBooking.time);
                bookingTimespan = timeAdd(bookingTimespan, currBookingDuration);
                minimumTime = timeAdd(endBooking.time, QTime(0, 1));

                while(timeAssignmentTime < bookingTimespan)
                {
                    if(timeAssignmentsIter == m_timeAssignments.constEnd())
                    {
                        errorMessage = tr("Missing time assignment! Missing: %0")
                                .arg(tr("%0h").arg(timeBetween(timeAssignmentTime, bookingTimespan).toString(tr("HH:mm:ss"))));

                        appendBookingEndStrip(endBooking.id, endBooking.time, currBookingDuration);

                        goto after;
                    }

                    timeAssignment = *timeAssignmentsIter++;

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
                    errorMessage = tr("Time assignment time longer than booking time!\nTime assignment: %0\nBooking: %1")
                            .arg(timeAssignmentTime.toString(tr("HH:mm:ss")))
                            .arg(bookingTimespan.toString(tr("HH:mm:ss")));
                }

                appendBookingEndStrip(endBooking.id, endBooking.time, currBookingDuration);

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
                                .arg(tr("%0h").arg(timeAssignmentTime.toString(tr("HH:mm")))), this);
        m_stripsLayout->addWidget(label);
    }
    else
    {
        startEnabled = false;
        endEnabled = false;

        timeAssignmentTime = QTime();
        lastTimeAssignmentStart = QTime();

        auto label = new QLabel(tr("Strip rendering aborted due error.\n"
                                   "Your bookings and time assignments for this day are in an illegal state!") % "\n" %
                                   errorMessage, this);
        label->setStyleSheet(QStringLiteral("color: red;"));
        m_stripsLayout->addWidget(label);
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

    return !errorMessage.isEmpty();
}

void StripsWidget::clearStrips()
{
    while(QLayoutItem *item = m_stripsLayout->takeAt(0))
    {
        delete item->widget();
        delete item;
    }
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

QString StripsWidget::buildProjectString(const QString &project) const
{
    if(m_mainWindow.projects().contains(project))
        return m_mainWindow.projects().value(project) % "\n" % project;
    else
    {
        qWarning() << "could not find project" << project;
        return project;
    }
}

QWidget *StripsWidget::appendBookingStartStrip(int id, const QTime &time)
{
    auto widget = m_mainWindow.stripFactory().createBookingStartStrip(this).release();

    if(auto labelTime = widget->findChild<QWidget*>(QStringLiteral("labelTime")))
        labelTime->setProperty("text", time.toString(tr("HH:mm")));
    else
        qWarning() << "no labelTime found!";

    if(auto labelId = widget->findChild<QWidget*>(QStringLiteral("labelId")))
        labelId->setProperty("text", QString::number(id));
    else
        qWarning() << "no labelId found!";

    m_stripsLayout->addWidget(widget);

    return widget;
}

QWidget *StripsWidget::appendBookingEndStrip(int id, const QTime &time, const QTime &duration)
{
    auto widget = m_mainWindow.stripFactory().createBookingEndStrip(this).release();

    if(auto labelTime = widget->findChild<QWidget*>(QStringLiteral("labelTime")))
        labelTime->setProperty("text", time.toString(tr("HH:mm")));
    else
        qWarning() << "no labelTime found!";

    if(auto labelDuration = widget->findChild<QWidget*>(QStringLiteral("labelDuration")))
        labelDuration->setProperty("text", tr("%0h").arg(duration.toString(tr("HH:mm"))));
    else
        qWarning() << "no labelDuration found!";

    if(auto labelId = widget->findChild<QWidget*>(QStringLiteral("labelId")))
        labelId->setProperty("text", QString::number(id));
    else
        qWarning() << "no labelId found!";

    m_stripsLayout->addWidget(widget);

    return widget;
}

QWidget *StripsWidget::appendTimeAssignmentStrip(int id, const QTime &duration, const QString &project, const QString &subproject, const QString &workpackage, const QString &text)
{
    auto widget = m_mainWindow.stripFactory().createTimeAssignmentStrip(this).release();

    if(auto labelTime = widget->findChild<QWidget*>(QStringLiteral("labelTime")))
        labelTime->setProperty("text", duration == QTime(0, 0) ? tr("Open") : duration.toString(tr("HH:mm")));
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

    m_stripsLayout->addWidget(widget);

    return widget;
}
