#include "zeiterfassungapi.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringBuilder>

#include "replies/createbookingreply.h"
#include "replies/createtimeassignmentreply.h"
#include "replies/deletebookingreply.h"
#include "replies/deletetimeassignment.h"
#include "replies/getauswertungreply.h"
#include "replies/getbookingsreply.h"
#include "replies/getprojectsreply.h"
#include "replies/gettimeassignmentsreply.h"
#include "replies/loginpagereply.h"
#include "replies/loginreply.h"
#include "replies/updatebookingreply.h"
#include "replies/updatetimeassignmentreply.h"
#include "replies/userinforeply.h"

ZeiterfassungApi::ZeiterfassungApi(const QString &url, QObject *parent) :
    QObject(parent),
    m_url(url),
    m_manager(new QNetworkAccessManager(this)),
    m_replies { Q_NULLPTR, Q_NULLPTR, Q_NULLPTR, Q_NULLPTR, Q_NULLPTR, Q_NULLPTR,
                Q_NULLPTR, Q_NULLPTR, Q_NULLPTR, Q_NULLPTR, Q_NULLPTR, Q_NULLPTR,
                Q_NULLPTR }
{
}

const QString &ZeiterfassungApi::url() const
{
    return m_url;
}

void ZeiterfassungApi::setUrl(const QString &url)
{
    if(m_url != url)
        Q_EMIT urlChanged(m_url = url);
}

QNetworkAccessManager *ZeiterfassungApi::manager() const
{
    return m_manager;
}

LoginPageReply *ZeiterfassungApi::doLoginPage()
{
    QNetworkRequest request(QUrl(m_url % "pages/login.jsp"));

    auto reply = m_manager->get(request);

    return new LoginPageReply(reply, this);
}

LoginReply *ZeiterfassungApi::doLogin(const QString &username, const QString &password)
{
    QNetworkRequest request(QUrl(m_url % "pages/j_spring_security_check"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArrayLiteral("application/x-www-form-urlencoded"));
    request.setMaximumRedirectsAllowed(0);

    auto data = QStringLiteral("j_username=%0&j_password=%1&login=Anmelden").arg(username).arg(password).toUtf8();

    auto reply = m_manager->post(request, data);

    return new LoginReply(reply, this);
}

UserInfoReply *ZeiterfassungApi::doUserInfo()
{
    QNetworkRequest request(QUrl(m_url % "json/evoAppsUserInfoDialogController/load-EvoAppsUserInfoTO"));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("home"));

    auto reply = m_manager->get(request);

    return new UserInfoReply(reply, this);
}

GetBookingsReply *ZeiterfassungApi::doGetBookings(int userId, const QDate &start, const QDate &end)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/bookings?start=%1&end=%2&pnrLst=%3")
                                 .arg(m_url)
                                 .arg(start.toString(QStringLiteral("yyyyMMdd")))
                                 .arg(end.toString(QStringLiteral("yyyyMMdd")))
                                 .arg(userId)));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    auto reply = m_manager->get(request);

    return new GetBookingsReply(reply, this);
}

CreateBookingReply *ZeiterfassungApi::doCreateBooking(int userId, const QDate &date, const QTime &time, const QTime &timespan, const QString &type, const QString &text)
{
    QNetworkRequest request(QUrl(m_url % "json/booking"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArrayLiteral("application/json"));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    QJsonObject obj;
    obj[QStringLiteral("persNr")] = userId;
    obj[QStringLiteral("bookingDate")] = date.toString("yyyyMMdd").toInt();
    obj[QStringLiteral("bookingTime")] = time.toString("Hmmss").toInt();
    obj[QStringLiteral("bookingTimespan")] = timespan.toString("Hmmss").toInt();
    obj[QStringLiteral("bookingType")] = type;
    obj[QStringLiteral("hourCategory")] = QStringLiteral("");
    obj[QStringLiteral("empfEinh")] = QStringLiteral("");
    obj[QStringLiteral("bewEinh")] = QStringLiteral("");
    obj[QStringLiteral("text")] = text;

    auto reply = m_manager->post(request, QJsonDocument(obj).toJson());

    return new CreateBookingReply(reply, this);
}

UpdateBookingReply *ZeiterfassungApi::doUpdateBooking(int bookingId, int userId, const QDate &date, const QTime &time, const QTime &timespan, const QString &type, const QString &text)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/booking/%1").arg(m_url).arg(bookingId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArrayLiteral("application/json"));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    QJsonObject obj;
    obj[QStringLiteral("bookingNr")] = bookingId;
    obj[QStringLiteral("persNr")] = userId;
    obj[QStringLiteral("bookingDate")] = date.toString("yyyyMMdd").toInt();
    obj[QStringLiteral("bookingTime")] = time.toString("Hmmss").toInt();
    obj[QStringLiteral("bookingTimespan")] = timespan.toString("Hmmss").toInt();
    obj[QStringLiteral("bookingType")] = type;
    obj[QStringLiteral("hourCategory")] = QStringLiteral("");
    obj[QStringLiteral("empfEinh")] = QStringLiteral("");
    obj[QStringLiteral("bewEinh")] = QStringLiteral("");
    obj[QStringLiteral("text")] = text;

    auto data = QJsonDocument(obj).toJson();

    auto reply = m_manager->put(request, data);

    return new UpdateBookingReply(reply, this);
}

DeleteBookingReply *ZeiterfassungApi::doDeleteBooking(int bookingId)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/booking/%1?text=")
                                 .arg(m_url)
                                 .arg(bookingId)));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    m_replies.deleteBooking = m_manager->deleteResource(request);

    return new DeleteBookingReply(reply, this);
}

GetTimeAssignmentsReply *ZeiterfassungApi::doGetTimeAssignments(int userId, const QDate &start, const QDate &end)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/azebooking?start=%1&end=%2&pnrLst=%3")
                                 .arg(m_url)
                                 .arg(start.toString(QStringLiteral("yyyyMMdd")))
                                 .arg(end.toString(QStringLiteral("yyyyMMdd")))
                                 .arg(userId)));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    auto reply = m_manager->get(request);

    return new GetTimeAssignmentsReply(reply, this);
}

CreateTimeAssignmentReply *ZeiterfassungApi::doCreateTimeAssignment(int userId, const QDate &date, const QTime &time, const QTime &timespan,
                                           const QString &project, const QString &subproject, const QString &workpackage,
                                           const QString &text)
{
    QNetworkRequest request(QUrl(m_url % "json/azebooking"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArrayLiteral("application/json"));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    QJsonObject obj;
    obj[QStringLiteral("bookingNr")] = QJsonValue::Null;
    obj[QStringLiteral("persNr")] = userId;
    obj[QStringLiteral("bookingDate")] = date.toString("yyyyMMdd").toInt();
    obj[QStringLiteral("bookingTime")] = time.toString("Hmmss").toInt();
    obj[QStringLiteral("bookingTimespan")] = timespan.toString("Hmmss").toInt();
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

    auto reply = m_manager->post(request, data);

    return new CreateTimeAssignmentReply(reply, this);
}

UpdateTimeAssignmentReply *ZeiterfassungApi::doUpdateTimeAssignment(int timeAssignmentId, int userId, const QDate &date, const QTime &time,
                                           const QTime &timespan, const QString &project, const QString &subproject,
                                           const QString &workpackage, const QString &text)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/azebooking/%1").arg(m_url).arg(timeAssignmentId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArrayLiteral("application/json"));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    QJsonObject obj;
    obj[QStringLiteral("bookingNr")] = timeAssignmentId;
    obj[QStringLiteral("persNr")] = userId;
    obj[QStringLiteral("bookingDate")] = date.toString("yyyyMMdd").toInt();
    obj[QStringLiteral("bookingTime")] = time.toString("Hmmss").toInt();
    obj[QStringLiteral("bookingTimespan")] = timespan.toString("Hmmss").toInt();
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

    auto reply = m_manager->put(request, data);

    return new UpdateTimeAssignmentReply(reply, this);
}

DeleteTimeAssignmentReply *ZeiterfassungApi::doDeleteTimeAssignment(int timeAssignmentId)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/azebooking/%1")
                                 .arg(m_url)
                                 .arg(timeAssignmentId)));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    auto reply = m_manager->deleteResource(request);

    return new DeleteTimeAssignmentReply(reply,this);
}

GetProjectsReply *ZeiterfassungApi::doGetProjects(int userId, const QDate &date)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/combobox?persnr=%1&date=%2&dqkey=KOST&kowert0=&kowert1=&kowert2=&term=")
                                 .arg(m_url)
                                 .arg(userId)
                                 .arg(date.toString(QStringLiteral("yyyyMMdd")))));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    auto reply = m_manager->get(request);

    return new GetProjectsReply(reply, this);
}

GetAuswertungReply *ZeiterfassungApi::doGetAuswertung(int userId, const QDate &date)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/auswertung/month?persNr=%1&date=%2")
                                 .arg(m_url)
                                 .arg(userId)
                                 .arg(date.toString(QStringLiteral("yyyyMMdd")))));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    auto reply = m_manager->get(request);

    return new GetAuswertungReply(reply, this);
}

void ZeiterfassungApi::updateTimeAssignmentRequestFinished()
{
    if(m_replies.updateTimeAssignment->error() != QNetworkReply::NoError)
    {
        Q_EMIT updateTimeAssignmentFinished(false, tr("Request error occured: %0").arg(m_replies.updateTimeAssignment->error()), -1);
        goto end;
    }

    {
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(m_replies.updateTimeAssignment->readAll(), &error);
        if(error.error != QJsonParseError::NoError)
        {
            Q_EMIT updateTimeAssignmentFinished(false, tr("Parsing JSON failed: %0").arg(error.errorString()), -1);
            goto end;
        }

        if(!document.isObject())
        {
            Q_EMIT updateTimeAssignmentFinished(false, tr("JSON document is not an object!"), 0);
            goto end;
        }

        auto obj = document.object();

        if(!obj.contains(QStringLiteral("bookingNr")))
        {
            Q_EMIT updateTimeAssignmentFinished(false, tr("JSON does not contain bookingNr!"), 0);
            goto end;
        }

        auto timeAssignmentId = obj.value(QStringLiteral("bookingNr")).toInt();

        Q_EMIT updateTimeAssignmentFinished(true, QString(), timeAssignmentId);
    }

    end:
    m_replies.updateTimeAssignment->deleteLater();
    m_replies.updateTimeAssignment = Q_NULLPTR;
}

void ZeiterfassungApi::deleteTimeAssignmentRequestFinished()
{
    if(m_replies.deleteTimeAssignment->error() != QNetworkReply::NoError)
    {
        Q_EMIT deleteTimeAssignmentFinished(false, tr("Request error occured: %0").arg(m_replies.deleteTimeAssignment->error()));
        goto end;
    }

    //only contains deleted id, so nothing to check here
    Q_EMIT deleteTimeAssignmentFinished(true, QString());

    end:
    m_replies.deleteTimeAssignment->deleteLater();
    m_replies.deleteTimeAssignment = Q_NULLPTR;
}

void ZeiterfassungApi::getProjectsRequestFinished()
{
    if(m_replies.getProjects->error() != QNetworkReply::NoError)
    {
        Q_EMIT getProjectsFinished(false, tr("Request error occured: %0").arg(m_replies.getProjects->error()), {});
        goto end;
    }

    {
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(m_replies.getProjects->readAll(), &error);
        if(error.error != QJsonParseError::NoError)
        {
            Q_EMIT getProjectsFinished(false, tr("Parsing JSON failed: %0").arg(error.errorString()), {});
            goto end;
        }

        if(!document.isObject())
        {
            Q_EMIT getProjectsFinished(false, tr("JSON document is not an object!"), {});
            goto end;
        }

        auto rootObj = document.object();

        if(!rootObj.contains(QStringLiteral("elements")))
        {
            Q_EMIT getProjectsFinished(false, tr("JSON does not contain elements!"), {});
            goto end;
        }

        auto elements = rootObj.value(QStringLiteral("elements"));

        if(!elements.isArray())
        {
            Q_EMIT getProjectsFinished(false, tr("elements is not an array!"), {});
            goto end;
        }

        auto elementsArr = elements.toArray();
        QVector<Project> projects;

        for(const auto &val : elementsArr)
        {
            auto obj = val.toObject();

            projects.append({
                                obj.value(QStringLiteral("label")).toString(),
                                obj.value(QStringLiteral("value")).toString()
                            });
        }

        Q_EMIT getProjectsFinished(true, QString(), projects);
    }

    end:
    m_replies.getProjects->deleteLater();
    m_replies.getProjects = Q_NULLPTR;
}

void ZeiterfassungApi::getAuswertungRequest0Finished()
{
    if(m_replies.getAuswertung->error() != QNetworkReply::NoError)
    {
        Q_EMIT getAuswertungFinished(false, tr("Request error occured: %0").arg(m_replies.getAuswertung->error()), QByteArray());
        m_replies.getAuswertung->deleteLater();
        m_replies.getAuswertung = Q_NULLPTR;
        return;
    }

    QUrl url(m_url);
    url.setPath(QString(m_replies.getAuswertung->readAll()));

    m_replies.getAuswertung->deleteLater();

    m_replies.getAuswertung = m_manager->get(QNetworkRequest(url));
    connect(m_replies.getAuswertung, &QNetworkReply::finished,
            this,                    &ZeiterfassungApi::getAuswertungRequest1Finished);
}

void ZeiterfassungApi::getAuswertungRequest1Finished()
{
    if(m_replies.getAuswertung->error() != QNetworkReply::NoError)
    {
        Q_EMIT getAuswertungFinished(false, tr("Request error occured: %0").arg(m_replies.getAuswertung->error()), QByteArray());
        goto end;
    }

    Q_EMIT getAuswertungFinished(true, QString(), m_replies.getAuswertung->readAll());

    end:
    m_replies.getAuswertung->deleteLater();
    m_replies.getAuswertung = Q_NULLPTR;
}
