#include "zeiterfassungapi.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringBuilder>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>

#include "replies/createbookingreply.h"
#include "replies/createtimeassignmentreply.h"
#include "replies/deletebookingreply.h"
#include "replies/deletetimeassignmentreply.h"
#include "replies/getauswertungreply.h"
#include "replies/getbookingsreply.h"
#include "replies/getpresencestatusreply.h"
#include "replies/getprojectsreply.h"
#include "replies/gettimeassignmentsreply.h"
#include "replies/loginpagereply.h"
#include "replies/loginreply.h"
#include "replies/updatebookingreply.h"
#include "replies/updatetimeassignmentreply.h"
#include "replies/userinforeply.h"

//add support for pre cpp14 compilers
#include "cpp14polyfills.h"

ZeiterfassungApi::ZeiterfassungApi(const QString &url, QObject *parent) :
    QObject(parent),
    m_url(url),
    m_manager(new QNetworkAccessManager(this))
{
}

const QString &ZeiterfassungApi::url() const
{
    return m_url;
}

void ZeiterfassungApi::setUrl(const QString &url)
{
    m_url = url;
}

QNetworkAccessManager *ZeiterfassungApi::manager() const
{
    return m_manager;
}

std::unique_ptr<LoginPageReply> ZeiterfassungApi::doLoginPage()
{
    QNetworkRequest request(QUrl(m_url % "pages/login.jsp"));

    auto reply = std::unique_ptr<QNetworkReply>(m_manager->get(request));

    return std::make_unique<LoginPageReply>(std::move(reply), this);
}

std::unique_ptr<LoginReply> ZeiterfassungApi::doLogin(const QString &username, const QString &password)
{
    QNetworkRequest request(QUrl(m_url % "pages/j_spring_security_check"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArrayLiteral("application/x-www-form-urlencoded"));
    request.setMaximumRedirectsAllowed(0);

    auto data = QStringLiteral("j_username=%0&j_password=%1&login=Anmelden").arg(username).arg(password).toUtf8();

    auto reply = std::unique_ptr<QNetworkReply>(m_manager->post(request, data));

    return std::make_unique<LoginReply>(std::move(reply), this);
}

std::unique_ptr<UserInfoReply> ZeiterfassungApi::doUserInfo()
{
    QNetworkRequest request(QUrl(m_url % "json/evoAppsUserInfoDialogController/load-EvoAppsUserInfoTO"));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("home"));

    auto reply = std::unique_ptr<QNetworkReply>(m_manager->get(request));

    return std::make_unique<UserInfoReply>(std::move(reply), this);
}

std::unique_ptr<GetBookingsReply> ZeiterfassungApi::doGetBookings(int userId, const QDate &start, const QDate &end)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/bookings?start=%1&end=%2&pnrLst=%3")
                                 .arg(m_url)
                                 .arg(start.toString(QStringLiteral("yyyyMMdd")))
                                 .arg(end.toString(QStringLiteral("yyyyMMdd")))
                                 .arg(userId)));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    auto reply = std::unique_ptr<QNetworkReply>(m_manager->get(request));

    return std::make_unique<GetBookingsReply>(std::move(reply), this);
}

std::unique_ptr<CreateBookingReply> ZeiterfassungApi::doCreateBooking(int userId, const QDate &date, const QTime &time, const QTime &timespan,
                                                                      const QString &type, const QString &text)
{
    QNetworkRequest request(QUrl(m_url % "json/booking"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArrayLiteral("application/json"));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    QJsonObject obj;
    obj[QStringLiteral("persNr")] = userId;
    obj[QStringLiteral("bookingDate")] = date.toString(QStringLiteral("yyyyMMdd")).toInt();
    obj[QStringLiteral("bookingTime")] = time.toString(QStringLiteral("Hmmss")).toInt();
    obj[QStringLiteral("bookingTimespan")] = timespan.toString(QStringLiteral("Hmmss")).toInt();
    obj[QStringLiteral("bookingType")] = type;
    obj[QStringLiteral("hourCategory")] = QStringLiteral("");
    obj[QStringLiteral("empfEinh")] = QStringLiteral("");
    obj[QStringLiteral("bewEinh")] = QStringLiteral("");
    obj[QStringLiteral("text")] = text;

    auto reply = std::unique_ptr<QNetworkReply>(m_manager->post(request, QJsonDocument(obj).toJson()));

    return std::make_unique<CreateBookingReply>(std::move(reply), this);
}

std::unique_ptr<UpdateBookingReply> ZeiterfassungApi::doUpdateBooking(int bookingId, int userId, const QDate &date, const QTime &time,
                                                                      const QTime &timespan, const QString &type, const QString &text)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/booking/%1").arg(m_url).arg(bookingId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArrayLiteral("application/json"));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    QJsonObject obj;
    obj[QStringLiteral("bookingNr")] = bookingId;
    obj[QStringLiteral("persNr")] = userId;
    obj[QStringLiteral("bookingDate")] = date.toString(QStringLiteral("yyyyMMdd")).toInt();
    obj[QStringLiteral("bookingTime")] = time.toString(QStringLiteral("Hmmss")).toInt();
    obj[QStringLiteral("bookingTimespan")] = timespan.toString(QStringLiteral("Hmmss")).toInt();
    obj[QStringLiteral("bookingType")] = type;
    obj[QStringLiteral("hourCategory")] = QStringLiteral("");
    obj[QStringLiteral("empfEinh")] = QStringLiteral("");
    obj[QStringLiteral("bewEinh")] = QStringLiteral("");
    obj[QStringLiteral("text")] = text;

    auto data = QJsonDocument(obj).toJson();

    auto reply = std::unique_ptr<QNetworkReply>(m_manager->put(request, data));

    return std::make_unique<UpdateBookingReply>(std::move(reply), this);
}

std::unique_ptr<DeleteBookingReply> ZeiterfassungApi::doDeleteBooking(int bookingId)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/booking/%1?text=")
                                 .arg(m_url)
                                 .arg(bookingId)));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    auto reply = std::unique_ptr<QNetworkReply>(m_manager->deleteResource(request));

    return std::make_unique<DeleteBookingReply>(std::move(reply), this);
}

std::unique_ptr<GetTimeAssignmentsReply> ZeiterfassungApi::doGetTimeAssignments(int userId, const QDate &start, const QDate &end)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/azebooking?start=%1&end=%2&pnrLst=%3")
                                 .arg(m_url)
                                 .arg(start.toString(QStringLiteral("yyyyMMdd")))
                                 .arg(end.toString(QStringLiteral("yyyyMMdd")))
                                 .arg(userId)));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    auto reply = std::unique_ptr<QNetworkReply>(m_manager->get(request));

    return std::make_unique<GetTimeAssignmentsReply>(std::move(reply), this);
}

std::unique_ptr<CreateTimeAssignmentReply> ZeiterfassungApi::doCreateTimeAssignment(int userId, const QDate &date, const QTime &time,
                                                                                    const QTime &timespan, const QString &project,
                                                                                    const QString &subproject, const QString &workpackage,
                                                                                    const QString &text)
{
    QNetworkRequest request(QUrl(m_url % "json/azebooking"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArrayLiteral("application/json"));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    QJsonObject obj;
    obj[QStringLiteral("bookingNr")] = QJsonValue::Null;
    obj[QStringLiteral("persNr")] = userId;
    obj[QStringLiteral("bookingDate")] = date.toString(QStringLiteral("yyyyMMdd")).toInt();
    obj[QStringLiteral("bookingTime")] = time.toString(QStringLiteral("Hmmss")).toInt();
    obj[QStringLiteral("bookingTimespan")] = timespan.toString(QStringLiteral("Hmmss")).toInt();
    obj[QStringLiteral("text")] = text;
    {
        QJsonArray koWertList;
        {
            QJsonObject obj;
            obj[QStringLiteral("value")] = project;
            koWertList << obj;
        }
        {
            QJsonObject obj;
            obj[QStringLiteral("value")] = subproject;
            koWertList << obj;
        }
        {
            QJsonObject obj;
            obj[QStringLiteral("value")] = workpackage;
            koWertList << obj;
        }
        obj[QStringLiteral("koWertList")] = koWertList;
    }

    auto data = QJsonDocument(obj).toJson();

    auto reply = std::unique_ptr<QNetworkReply>(m_manager->post(request, data));

    return std::make_unique<CreateTimeAssignmentReply>(std::move(reply), this);
}

std::unique_ptr<UpdateTimeAssignmentReply> ZeiterfassungApi::doUpdateTimeAssignment(int timeAssignmentId, int userId, const QDate &date,
                                                                                    const QTime &time, const QTime &timespan, const QString &project,
                                                                                    const QString &subproject, const QString &workpackage,
                                                                                    const QString &text)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/azebooking/%1").arg(m_url).arg(timeAssignmentId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArrayLiteral("application/json"));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    QJsonObject obj;
    obj[QStringLiteral("bookingNr")] = timeAssignmentId;
    obj[QStringLiteral("persNr")] = userId;
    obj[QStringLiteral("bookingDate")] = date.toString(QStringLiteral("yyyyMMdd")).toInt();
    obj[QStringLiteral("bookingTime")] = time.toString(QStringLiteral("Hmmss")).toInt();
    obj[QStringLiteral("bookingTimespan")] = timespan.toString(QStringLiteral("Hmmss")).toInt();
    obj[QStringLiteral("bookingType")] = QJsonValue::Null;
    obj[QStringLiteral("hourCategory")] = QJsonValue::Null;
    obj[QStringLiteral("bewEinh")] = QJsonValue::Null;
    obj[QStringLiteral("empfEinh")] = QJsonValue::Null;
    obj[QStringLiteral("einstuf")] = 0;
    obj[QStringLiteral("text")] = text;
    {
        QJsonArray koWertList;
        {
            QJsonObject obj;
            obj[QStringLiteral("value")] = project;
            koWertList << obj;
        }
        {
            QJsonObject obj;
            obj[QStringLiteral("value")] = subproject;
            koWertList << obj;
        }
        {
            QJsonObject obj;
            obj[QStringLiteral("value")] = workpackage;
            koWertList << obj;
        }
        obj[QStringLiteral("koWertList")] = koWertList;
    }

    auto data = QJsonDocument(obj).toJson();

    auto reply = std::unique_ptr<QNetworkReply>(m_manager->put(request, data));

    return std::make_unique<UpdateTimeAssignmentReply>(std::move(reply), this);
}

std::unique_ptr<DeleteTimeAssignmentReply> ZeiterfassungApi::doDeleteTimeAssignment(int timeAssignmentId)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/azebooking/%1")
                                 .arg(m_url)
                                 .arg(timeAssignmentId)));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    auto reply = std::unique_ptr<QNetworkReply>(m_manager->deleteResource(request));

    return std::make_unique<DeleteTimeAssignmentReply>(std::move(reply), this);
}

std::unique_ptr<GetProjectsReply> ZeiterfassungApi::doGetProjects(int userId, const QDate &date)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/combobox?persnr=%1&date=%2&dqkey=KOST&kowert0=&kowert1=&kowert2=&term=")
                                 .arg(m_url)
                                 .arg(userId)
                                 .arg(date.toString(QStringLiteral("yyyyMMdd")))));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    auto reply = std::unique_ptr<QNetworkReply>(m_manager->get(request));

    return std::make_unique<GetProjectsReply>(std::move(reply), this);
}

std::unique_ptr<GetAuswertungReply> ZeiterfassungApi::doGetAuswertung(int userId, const QDate &date)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/auswertung/month?persNr=%1&date=%2")
                                 .arg(m_url)
                                 .arg(userId)
                                 .arg(date.toString(QStringLiteral("yyyyMMdd")))));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    auto reply = std::unique_ptr<QNetworkReply>(m_manager->get(request));

    return std::make_unique<GetAuswertungReply>(std::move(reply), this);
}

std::unique_ptr<GetPresenceStatusReply> ZeiterfassungApi::doGetPresenceStatus()
{
    QNetworkRequest request(QUrl(m_url % "json/presencestatus"));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("presenceStatus"));

    auto reply = std::unique_ptr<QNetworkReply>(m_manager->get(request));

    return std::make_unique<GetPresenceStatusReply>(std::move(reply), this);
}
