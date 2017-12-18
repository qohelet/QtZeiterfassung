#ifndef STRIPSWIDGET_H
#define STRIPSWIDGET_H

#include <memory>

#include <QWidget>
#include <QTime>

#include "zeiterfassunglib_global.h"
#include "replies/getbookingsreply.h"
#include "replies/gettimeassignmentsreply.h"

class QBoxLayout;
class QLabel;
template <typename T> class QVector;

class MainWindow;

class ZEITERFASSUNGLIBSHARED_EXPORT StripsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StripsWidget(MainWindow &mainWindow, QWidget *parent = Q_NULLPTR);

    MainWindow &mainWindow() const;

    QBoxLayout *headerLayout() const;
    QBoxLayout *stripsLayout() const;

    QLabel *label() const;

    const QDate &date() const;
    void setDate(const QDate &date);

    const QVector<GetBookingsReply::Booking> &bookings() const;
    const QVector<GetTimeAssignmentsReply::TimeAssignment> &timeAssignments() const;

    const QTime &timeAssignmentTime() const;
    const QTime &lastTimeAssignmentStart() const;
    const QTime &minimumTime() const;
    bool refreshing() const;
    bool refreshingBookings() const;
    bool refreshingTimeAssignments() const;
    bool startEnabled() const;
    bool endEnabled() const;

    void refresh();
    void refreshBookings(bool createLabel = true);
    void refreshTimeAssignments(bool createLabel = true);
    bool createStrips();
    void clearStrips();

Q_SIGNALS:
    void dateChanged(const QDate &date);

    void bookingsChanged(const QVector<GetBookingsReply::Booking> &bookings);
    void timeAssignmentsChanged(const QVector<GetTimeAssignmentsReply::TimeAssignment> &timeAssignments);

    void timeAssignmentTimeChanged(const QTime &timeAssignmentTime);
    void lastTimeAssignmentStartChanged(const QTime &lastTimeAssignmentStart);
    void minimumTimeChanged(const QTime &minimumTime);
    void refreshingChanged(bool refreshing);
    void refreshingBookingsChanged(bool refreshingBookings);
    void refreshingTimeAssignmentsChanged(bool refreshingTimeAssignments);
    void startEnabledChanged(bool startEnabled);
    void endEnabledChanged(bool endEnabled);

private Q_SLOTS:
    void getBookingsFinished();
    void getTimeAssignmentsFinished();

private:
    void invalidateValues();    
    QString buildProjectString(const QString &project) const;

    QWidget *appendBookingStartStrip(int id, const QTime &time);
    QWidget *appendBookingEndStrip(int id, const QTime &time);
    QWidget *appendTimeAssignmentStrip(int id, const QTime &duration, const QString &project, const QString &subproject,
                                       const QString &workpackage, const QString &text);

    MainWindow &m_mainWindow;

    QBoxLayout *m_headerLayout;
    QBoxLayout *m_stripsLayout;

    QLabel *m_label;

    QDate m_date;

    QVector<GetBookingsReply::Booking> m_bookings;
    QVector<GetTimeAssignmentsReply::TimeAssignment> m_timeAssignments;

    QTime m_timeAssignmentTime;
    QTime m_lastTimeAssignmentStart;
    QTime m_minimumTime;
    bool m_refreshing;
    bool m_refreshingBookings;
    bool m_refreshingTimeAssignments;
    bool m_startEnabled;
    bool m_endEnabled;

    std::unique_ptr<GetBookingsReply> m_getBookingsReply;
    std::unique_ptr<GetTimeAssignmentsReply> m_getTimeAssignmentsReply;
};

#endif // STRIPSWIDGET_H
