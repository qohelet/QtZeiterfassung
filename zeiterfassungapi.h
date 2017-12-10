#ifndef ZEITERFASSUNGAPI_H
#define ZEITERFASSUNGAPI_H

#include <QObject>
#include <QString>
#include <QDate>
#include <QTime>

class QNetworkAccessManager;

class LoginPageReply;
class LoginReply;
class UserInfoReply;
class GetBookingsReply;
class CreateBookingReply;
class UpdateBookingReply;
class DeleteBookingReply;
class GetTimeAssignmentsReply;
class CreateTimeAssignmentReply;
class UpdateTimeAssignmentReply;
class DeleteTimeAssignmentReply;
class GetProjectsReply;
class GetAuswertungReply;

class ZeiterfassungApi : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)

public:
    explicit ZeiterfassungApi(const QString &url, QObject *parent = Q_NULLPTR);

    const QString &url() const;
    void setUrl(const QString &url);

    QNetworkAccessManager *manager() const;

    struct UserInfo
    {
        int userId;
        QString email;
        QString longUsername;
        QString text;
        QString username;
    };

    struct Booking
    {
        int id;
        QDate date;
        QTime time;
        QTime timespan;
        QString type;
        QString text;
    };

    struct TimeAssignment
    {
        int id;
        QDate date;
        QTime time;
        QTime timespan;
        QString text;
        QString project;
        QString subproject;
        QString workpackage;
    };

    struct Project
    {
        QString label;
        QString value;
    };

public Q_SLOTS:
    LoginPageReply *doLoginPage();
    LoginReply *doLogin(const QString &username, const QString &password);
    UserInfoReply *doUserInfo();

    GetBookingsReply *doGetBookings(int userId, const QDate &start, const QDate &end);
    CreateBookingReply *doCreateBooking(int userId, const QDate &date, const QTime &time, const QTime &timespan,
                                   const QString &type, const QString &text);
    UpdateBookingReply *doUpdateBooking(int bookingId, int userId, const QDate &date, const QTime &time,
                                   const QTime &timespan, const QString &type, const QString &text);
    DeleteBookingReply *doDeleteBooking(int bookingId);

    GetTimeAssignmentsReply *doGetTimeAssignments(int userId, const QDate &start, const QDate &end);
    CreateTimeAssignmentReply *doCreateTimeAssignment(int userId, const QDate &date, const QTime &time, const QTime &timespan,
                                                 const QString &project, const QString &subproject, const QString &workpackage,
                                                 const QString &text);
    UpdateTimeAssignmentReply *doUpdateTimeAssignment(int timeAssignmentId, int userId, const QDate &date, const QTime &time,
                                                 const QTime &timespan, const QString &project, const QString &subproject,
                                                 const QString &workpackage, const QString &text);
    DeleteTimeAssignmentReply *doDeleteTimeAssignment(int timeAssignmentId);

    GetProjectsReply *doGetProjects(int userId, const QDate &date);
    GetAuswertungReply *doGetAuswertung(int userId, const QDate &date);

private:
    QString m_url;
    QNetworkAccessManager *m_manager;
};

#endif // ZEITERFASSUNGAPI_H
