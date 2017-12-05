#include "zeiterfassung.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QStringBuilder>

QString QJsonValue::toString() const{ return toString(QString()); }

Zeiterfassung::Zeiterfassung(const QString &url, QObject *parent) :
    QObject(parent),
    m_url(url),
    m_manager(new QNetworkAccessManager(this)),
    m_replies { Q_NULLPTR, Q_NULLPTR, Q_NULLPTR, Q_NULLPTR, Q_NULLPTR, Q_NULLPTR,
                Q_NULLPTR, Q_NULLPTR, Q_NULLPTR, Q_NULLPTR, Q_NULLPTR, Q_NULLPTR,
                Q_NULLPTR }
{
}

const QString &Zeiterfassung::url() const
{
    return m_url;
}

void Zeiterfassung::setUrl(const QString &url)
{
    if(m_url != url)
        Q_EMIT urlChanged(m_url = url);
}

bool Zeiterfassung::doLoginPage()
{
    if(m_replies.login)
    {
        qWarning() << "another loginPage already processing!";
        return false;
    }

    QNetworkRequest request(QUrl(m_url % "pages/login.jsp"));

    m_replies.loginPage = m_manager->get(request);
    connect(m_replies.loginPage, &QNetworkReply::finished, this, &Zeiterfassung::loginPageRequestFinished);

    return true;
}

bool Zeiterfassung::doLogin(const QString &username, const QString &password)
{
    if(m_replies.login)
    {
        qWarning() << "another login already processing!";
        return false;
    }

    QNetworkRequest request(QUrl(m_url % "pages/j_spring_security_check"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArrayLiteral("application/x-www-form-urlencoded"));
    request.setMaximumRedirectsAllowed(0);

    m_replies.login = m_manager->post(request, QStringLiteral("j_username=%0&j_password=%1&login=Anmelden").arg(username).arg(password).toUtf8());
    connect(m_replies.login, &QNetworkReply::finished, this, &Zeiterfassung::loginRequestFinished);

    return true;
}

bool Zeiterfassung::doUserInfo()
{
    if(m_replies.userInfo)
    {
        qWarning() << "another userInfo already processing!";
        return false;
    }

    QNetworkRequest request(QUrl(m_url % "json/evoAppsUserInfoDialogController/load-EvoAppsUserInfoTO"));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("home"));

    m_replies.userInfo = m_manager->get(request);
    connect(m_replies.userInfo, &QNetworkReply::finished, this, &Zeiterfassung::userInfoRequestFinished);

    return true;
}

bool Zeiterfassung::doGetBuchungen(int userId, const QDate &start, const QDate &end)
{
    if(m_replies.getBuchungen)
    {
        qWarning() << "another getBuchungen already processing!";
        return false;
    }

    QNetworkRequest request(QUrl(QStringLiteral("%0json/bookings?start=%1&end=%2&pnrLst=%3")
                                 .arg(m_url)
                                 .arg(start.toString(QStringLiteral("yyyyMMdd")))
                                 .arg(end.toString(QStringLiteral("yyyyMMdd")))
                                 .arg(userId)));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    m_replies.getBuchungen = m_manager->get(request);
    connect(m_replies.getBuchungen, &QNetworkReply::finished,
            this,                   &Zeiterfassung::getBuchungenRequestFinished);

    return true;
}

bool Zeiterfassung::doCreateBuchung(int userId, const QDate &date, const QTime &time, const QTime &timespan, const QString &type, const QString &text)
{
    if(m_replies.createBuchung)
    {
        qWarning() << "another createBuchung already processing!";
        return false;
    }

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

    m_replies.createBuchung = m_manager->post(request, QJsonDocument(obj).toJson());
    connect(m_replies.createBuchung, &QNetworkReply::finished,
            this,                    &Zeiterfassung::createBuchungRequestFinished);

    return true;
}

bool Zeiterfassung::doUpdateBuchung(int buchungId, int userId, const QDate &date, const QTime &time, const QTime &timespan, const QString &type, const QString &text)
{
    if(m_replies.updateBuchung)
    {
        qWarning() << "another updateBuchung already processing!";
        return false;
    }

    QNetworkRequest request(QUrl(QStringLiteral("%0json/booking/%1").arg(m_url).arg(buchungId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArrayLiteral("application/json"));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    QJsonObject obj;
    obj[QStringLiteral("bookingNr")] = buchungId;
    obj[QStringLiteral("persNr")] = userId;
    obj[QStringLiteral("bookingDate")] = date.toString("yyyyMMdd").toInt();
    obj[QStringLiteral("bookingTime")] = time.toString("Hmmss").toInt();
    obj[QStringLiteral("bookingTimespan")] = timespan.toString("Hmmss").toInt();
    obj[QStringLiteral("bookingType")] = type;
    obj[QStringLiteral("hourCategory")] = QStringLiteral("");
    obj[QStringLiteral("empfEinh")] = QStringLiteral("");
    obj[QStringLiteral("bewEinh")] = QStringLiteral("");
    obj[QStringLiteral("text")] = text;

    m_replies.updateBuchung = m_manager->put(request, QJsonDocument(obj).toJson());
    connect(m_replies.updateBuchung, &QNetworkReply::finished,
            this,                    &Zeiterfassung::updateBuchungRequestFinished);

    return true;
}

bool Zeiterfassung::doDeleteBuchung(int buchungId)
{
    if(m_replies.deleteBuchung)
    {
        qWarning() << "another deleteBuchung already processing!";
        return false;
    }

    QNetworkRequest request(QUrl(QStringLiteral("%0json/booking/%1?text=")
                                 .arg(m_url)
                                 .arg(buchungId)));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    m_replies.deleteBuchung = m_manager->deleteResource(request);
    connect(m_replies.deleteBuchung, &QNetworkReply::finished,
            this,                    &Zeiterfassung::deleteBuchungRequestFinished);

    return true;
}

bool Zeiterfassung::doGetTimeAssignments(int userId, const QDate &start, const QDate &end)
{
    if(m_replies.getTimeAssignments)
    {
        qWarning() << "another getTimeAssignments already processing!";
        return false;
    }

    QNetworkRequest request(QUrl(QStringLiteral("%0json/azebooking?start=%1&end=%2&pnrLst=%3")
                                 .arg(m_url)
                                 .arg(start.toString(QStringLiteral("yyyyMMdd")))
                                 .arg(end.toString(QStringLiteral("yyyyMMdd")))
                                 .arg(userId)));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    m_replies.getTimeAssignments = m_manager->get(request);
    connect(m_replies.getTimeAssignments, &QNetworkReply::finished,
            this,                      &Zeiterfassung::getTimeAssignmentsRequestFinished);

    return true;
}

bool Zeiterfassung::doCreateTimeAssignment(int userId, const QDate &date, const QTime &time, const QTime &timespan,
                                           const QString &projekt, const QString &subprojekt, const QString &workpackage,
                                           const QString &text)
{
    if(m_replies.createTimeAssignment)
    {
        qWarning() << "another createTimeAssignment already processing!";
        return false;
    }

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
            obj[QStringLiteral("value")] = projekt;
            koWertList << obj;
        }
        {
            QJsonObject obj;
            obj[QStringLiteral("value")] = subprojekt;
            koWertList << obj;
        }
        {
            QJsonObject obj;
            obj[QStringLiteral("value")] = workpackage;
            koWertList << obj;
        }
        obj[QStringLiteral("koWertList")] = koWertList;
    }

    m_replies.createTimeAssignment = m_manager->post(request, QJsonDocument(obj).toJson());
    connect(m_replies.createTimeAssignment, &QNetworkReply::finished,
            this,                       &Zeiterfassung::createTimeAssignmentRequestFinished);

    return true;
}

bool Zeiterfassung::doUpdateTimeAssignment(int timeAssignmentId, int userId, const QDate &date, const QTime &time,
                                           const QTime &timespan, const QString &projekt, const QString &subprojekt,
                                           const QString &workpackage, const QString &text)
{
    if(m_replies.updateTimeAssignment)
    {
        qWarning() << "another updateTimeAssignment already processing!";
        return false;
    }

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
            obj[QStringLiteral("value")] = projekt;
            koWertList << obj;
        }
        {
            QJsonObject obj;
            obj[QStringLiteral("value")] = subprojekt;
            koWertList << obj;
        }
        {
            QJsonObject obj;
            obj[QStringLiteral("value")] = workpackage;
            koWertList << obj;
        }
        obj[QStringLiteral("koWertList")] = koWertList;
    }

    m_replies.updateTimeAssignment = m_manager->put(request, QJsonDocument(obj).toJson());
    connect(m_replies.updateTimeAssignment, &QNetworkReply::finished,
            this,                       &Zeiterfassung::updateTimeAssignmentRequestFinished);

    return true;
}

bool Zeiterfassung::doDeleteTimeAssignment(int timeAssignmentId)
{
    if(m_replies.deleteTimeAssignment)
    {
        qWarning() << "another deleteTimeAssignment already processing!";
        return false;
    }

    QNetworkRequest request(QUrl(QStringLiteral("%0json/azebooking/%1")
                                 .arg(m_url)
                                 .arg(timeAssignmentId)));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    m_replies.deleteTimeAssignment = m_manager->deleteResource(request);
    connect(m_replies.deleteTimeAssignment, &QNetworkReply::finished,
            this,                       &Zeiterfassung::deleteTimeAssignmentRequestFinished);

    return true;
}

bool Zeiterfassung::doGetProjekte(int userId, const QDate &date)
{
    if(m_replies.getProjekte)
    {
        qWarning() << "another getProjekte already processing!";
        return false;
    }

    QNetworkRequest request(QUrl(QStringLiteral("%0json/combobox?persnr=%1&date=%2&dqkey=KOST&kowert0=&kowert1=&kowert2=&term=")
                                 .arg(m_url)
                                 .arg(userId)
                                 .arg(date.toString(QStringLiteral("yyyyMMdd")))));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    m_replies.getProjekte = m_manager->get(request);
    connect(m_replies.getProjekte, &QNetworkReply::finished, this, &Zeiterfassung::getProjekteRequestFinished);

    return true;
}

bool Zeiterfassung::doGetAuswertung(int userId, const QDate &date)
{
    if(m_replies.getAuswertung)
    {
        qWarning() << "another getAuswertung already processing!";
        return false;
    }

    QNetworkRequest request(QUrl(QStringLiteral("%0json/auswertung/month?persNr=%1&date=%2")
                                 .arg(m_url)
                                 .arg(userId)
                                 .arg(date.toString(QStringLiteral("yyyyMMdd")))));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    m_replies.getAuswertung = m_manager->get(request);
    connect(m_replies.getAuswertung, &QNetworkReply::finished, this, &Zeiterfassung::getAuswertungRequest0Finished);

    return true;
}

void Zeiterfassung::loginPageRequestFinished()
{
    if(m_replies.loginPage->error() != QNetworkReply::NoError)
    {
        Q_EMIT loginPageFinished(false, tr("Request error occured: %0").arg(m_replies.loginPage->error()));
        goto end;
    }

    if(!m_replies.loginPage->readAll().contains(QByteArrayLiteral("evoApps Anmeldung")))
    {
        Q_EMIT loginPageFinished(false, tr("Could not find necessary keywords in login page!"));
        goto end;
    }

    Q_EMIT loginPageFinished(true, QString());

    end:
    m_replies.loginPage->deleteLater();
    m_replies.loginPage = Q_NULLPTR;
}

void Zeiterfassung::loginRequestFinished()
{
    if(m_replies.login->error() != QNetworkReply::NoError)
    {
        Q_EMIT loginFinished(false, tr("Request error occured: %0").arg(m_replies.login->error()));
        goto end;
    }

    if(!m_replies.login->hasRawHeader(QByteArrayLiteral("Location")))
    {
        Q_EMIT loginFinished(false, tr("Request did not contain a Location header."));
        goto end;
    }

    {
        auto location = m_replies.login->rawHeader(QByteArrayLiteral("Location"));

        if(location == QByteArrayLiteral("/evoApps/pages/home.jsp"))
        {
            Q_EMIT loginFinished(true, QString());
            goto end;
        }
        else if(location == QByteArrayLiteral("/evoApps/pages/login.jsp?error=user"))
        {
            Q_EMIT loginFinished(false, tr("Authentication failure. Please check username and password."));
            goto end;
        }
        else
        {
            Q_EMIT loginFinished(false, tr("An unknown authentication failure occured. Redirected to: %0").arg(QString(location)));
            goto end;
        }
    }

    end:
    m_replies.login->deleteLater();
    m_replies.login = Q_NULLPTR;
}

void Zeiterfassung::userInfoRequestFinished()
{
    if(m_replies.userInfo->error() != QNetworkReply::NoError)
    {
        Q_EMIT userInfoFinished(false, tr("Request error occured: %0").arg(m_replies.userInfo->error()), UserInfo());
        goto end;
    }

    {
        QJsonParseError error;
        auto document = QJsonDocument::fromJson(m_replies.userInfo->readAll(), &error);
        if(error.error != QJsonParseError::NoError)
        {
            Q_EMIT userInfoFinished(false, tr("Parsing JSON failed: %0").arg(error.errorString()), UserInfo());
            goto end;
        }

        if(!document.isObject())
        {
            Q_EMIT userInfoFinished(false, tr("JSON document is not an object!"), UserInfo());
            goto end;
        }

        auto rootObj = document.object();

        if(!rootObj.contains(QStringLiteral("evoAppsUser")))
        {
            Q_EMIT userInfoFinished(false, tr("JSON does not contain evoAppsUser!"), UserInfo());
            goto end;
        }

        auto evoAppsUser = rootObj.value(QStringLiteral("evoAppsUser"));

        if(!evoAppsUser.isObject())
        {
            Q_EMIT userInfoFinished(false, tr("evoAppsUser is not an object!"), UserInfo());
            goto end;
        }

        auto evoAppsUserObj = evoAppsUser.toObject();

        Q_EMIT userInfoFinished(true, QString(), {
                                    evoAppsUserObj.value(QStringLiteral("persNr")).toInt(),
                                    evoAppsUserObj.value(QStringLiteral("email")).toString(),
                                    evoAppsUserObj.value(QStringLiteral("longUsername")).toString(),
                                    evoAppsUserObj.value(QStringLiteral("text")).toString(),
                                    evoAppsUserObj.value(QStringLiteral("username")).toString()
                                });
    }

    end:
    m_replies.userInfo->deleteLater();
    m_replies.userInfo = Q_NULLPTR;
}

void Zeiterfassung::getBuchungenRequestFinished()
{
    if(m_replies.getBuchungen->error() != QNetworkReply::NoError)
    {
        Q_EMIT getBuchungenFinished(false, tr("Request error occured: %0").arg(m_replies.getBuchungen->error()), {});
        goto end;
    }

    {
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(m_replies.getBuchungen->readAll(), &error);
        if(error.error != QJsonParseError::NoError)
        {
            Q_EMIT getBuchungenFinished(false, tr("Parsing JSON failed: %0").arg(error.errorString()), {});
            goto end;
        }

        if(!document.isArray())
        {
            Q_EMIT getBuchungenFinished(false, tr("JSON document is not an array!"), {});
            goto end;
        }

        auto arr = document.array();
        QVector<Buchung> buchungen;

        for(const auto &val : arr)
        {
            auto obj = val.toObject();

            buchungen.append({
                                 obj.value(QStringLiteral("bookingNr")).toInt(),
                                 QDate::fromString(QString::number(obj.value(QStringLiteral("bookingDate")).toInt()), QStringLiteral("yyyyMMdd")),
                                 QTime::fromString(QString("%0").arg(obj.value(QStringLiteral("bookingTime")).toInt(), 6, 10, QChar('0')), QStringLiteral("HHmmss")),
                                 QTime::fromString(QString("%0").arg(obj.value(QStringLiteral("bookingTimespan")).toInt(), 6, 10, QChar('0')), QStringLiteral("HHmmss")),
                                 obj.value(QStringLiteral("bookingType")).toString(),
                                 obj.value(QStringLiteral("text")).toString()
                             });
        }

        Q_EMIT getBuchungenFinished(true, QString(), buchungen);
    }

    end:
    m_replies.getBuchungen->deleteLater();
    m_replies.getBuchungen = Q_NULLPTR;
}

void Zeiterfassung::createBuchungRequestFinished()
{
    if(m_replies.createBuchung->error() != QNetworkReply::NoError)
    {
        Q_EMIT createBuchungFinished(false, tr("Request error occured: %0").arg(m_replies.createBuchung->error()), -1);
        goto end;
    }

    {
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(m_replies.createBuchung->readAll(), &error);
        if(error.error != QJsonParseError::NoError)
        {
            Q_EMIT createBuchungFinished(false, tr("Parsing JSON failed: %0").arg(error.errorString()), -1);
            goto end;
        }

        if(!document.isObject())
        {
            Q_EMIT createBuchungFinished(false, tr("JSON document is not an object!"), -1);
            goto end;
        }

        auto obj = document.object();

        if(!obj.contains(QStringLiteral("bookingNr")))
        {
            Q_EMIT createBuchungFinished(false, tr("JSON does not contain bookingNr!"), -1);
            goto end;
        }

        auto buchungId = obj.value(QStringLiteral("bookingNr")).toInt();

        Q_EMIT createBuchungFinished(true, QString(), buchungId);
    }

    end:
    m_replies.createBuchung->deleteLater();
    m_replies.createBuchung = Q_NULLPTR;
}

void Zeiterfassung::updateBuchungRequestFinished()
{
    if(m_replies.updateBuchung->error() != QNetworkReply::NoError)
    {
        Q_EMIT updateBuchungFinished(false, tr("Request error occured: %0").arg(m_replies.updateBuchung->error()), -1);
        goto end;
    }

    {
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(m_replies.updateBuchung->readAll(), &error);
        if(error.error != QJsonParseError::NoError)
        {
            Q_EMIT updateBuchungFinished(false, tr("Parsing JSON failed: %0").arg(error.errorString()), -1);
            goto end;
        }

        if(!document.isObject())
        {
            Q_EMIT updateBuchungFinished(false, tr("JSON document is not an object!"), -1);
            goto end;
        }

        auto obj = document.object();

        if(!obj.contains(QStringLiteral("bookingNr")))
        {
            Q_EMIT updateBuchungFinished(false, tr("JSON does not contain bookingNr!"), -1);
            goto end;
        }

        auto buchungId = obj.value(QStringLiteral("bookingNr")).toInt();

        Q_EMIT updateBuchungFinished(true, QString(), buchungId);
    }

    end:
    m_replies.updateBuchung->deleteLater();
    m_replies.updateBuchung = Q_NULLPTR;
}

void Zeiterfassung::deleteBuchungRequestFinished()
{
    if(m_replies.deleteBuchung->error() != QNetworkReply::NoError)
    {
        Q_EMIT deleteBuchungFinished(false, tr("Request error occured: %0").arg(m_replies.deleteBuchung->error()));
        goto end;
    }

    //should be empty, so nothing to check...
    Q_EMIT deleteBuchungFinished(true, QString());

    end:
    m_replies.deleteBuchung->deleteLater();
    m_replies.deleteBuchung = Q_NULLPTR;
}

void Zeiterfassung::getTimeAssignmentsRequestFinished()
{
    if(m_replies.getTimeAssignments->error() != QNetworkReply::NoError)
    {
        Q_EMIT getTimeAssignmentsFinished(false, tr("Request error occured: %0").arg(m_replies.getTimeAssignments->error()), {});
        goto end;
    }

    {
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(m_replies.getTimeAssignments->readAll(), &error);
        if(error.error != QJsonParseError::NoError)
        {
            Q_EMIT getTimeAssignmentsFinished(false, tr("Parsing JSON failed: %0").arg(error.errorString()), {});
            goto end;
        }

        if(!document.isArray())
        {
            Q_EMIT getTimeAssignmentsFinished(false, tr("JSON document is not an array!"), {});
            goto end;
        }

        auto arr = document.array();
        QVector<TimeAssignment> timeAssignments;

        for(const auto &val : arr)
        {
            auto obj = val.toObject();

            auto koWertList = obj.value(QStringLiteral("koWertList")).toArray();

            timeAssignments.append({
                                    obj.value(QStringLiteral("bookingNr")).toInt(),
                                    QDate::fromString(QString::number(obj.value(QStringLiteral("bookingDate")).toInt()), QStringLiteral("yyyyMMdd")),
                                    QTime::fromString(QString("%0").arg(obj.value(QStringLiteral("bookingTime")).toInt(), 6, 10, QChar('0')), QStringLiteral("HHmmss")),
                                    QTime::fromString(QString("%0").arg(obj.value(QStringLiteral("bookingTimespan")).toInt(), 6, 10, QChar('0')), QStringLiteral("HHmmss")),
                                    obj.value(QStringLiteral("text")).toString(),
                                    koWertList.at(0).toObject().value(QStringLiteral("value")).toString(),
                                    koWertList.at(1).toObject().value(QStringLiteral("value")).toString(),
                                    koWertList.at(2).toObject().value(QStringLiteral("value")).toString()
                               });
        }

        Q_EMIT getTimeAssignmentsFinished(true, QString(), timeAssignments);
    }

    end:
    m_replies.getTimeAssignments->deleteLater();
    m_replies.getTimeAssignments = Q_NULLPTR;
}

void Zeiterfassung::createTimeAssignmentRequestFinished()
{
    if(m_replies.createTimeAssignment->error() != QNetworkReply::NoError)
    {
        Q_EMIT createTimeAssignmentFinished(false, tr("Request error occured: %0").arg(m_replies.createTimeAssignment->error()), -1);
        goto end;
    }

    {
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(m_replies.createTimeAssignment->readAll(), &error);
        if(error.error != QJsonParseError::NoError)
        {
            Q_EMIT createTimeAssignmentFinished(false, tr("Parsing JSON failed: %0").arg(error.errorString()), -1);
            goto end;
        }

        if(!document.isObject())
        {
            Q_EMIT createTimeAssignmentFinished(false, tr("JSON document is not an object!"), -1);
            goto end;
        }

        auto obj = document.object();

        if(!obj.contains(QStringLiteral("bookingNr")))
        {
            Q_EMIT createTimeAssignmentFinished(false, tr("JSON does not contain bookingNr!"), -1);
            goto end;
        }

        auto timeAssignmentId = obj.value(QStringLiteral("bookingNr")).toInt();

        Q_EMIT createTimeAssignmentFinished(true, QString(), timeAssignmentId);
    }

    end:
    m_replies.createTimeAssignment->deleteLater();
    m_replies.createTimeAssignment = Q_NULLPTR;
}

void Zeiterfassung::updateTimeAssignmentRequestFinished()
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

void Zeiterfassung::deleteTimeAssignmentRequestFinished()
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

void Zeiterfassung::getProjekteRequestFinished()
{
    if(m_replies.getProjekte->error() != QNetworkReply::NoError)
    {
        Q_EMIT getProjekteFinished(false, tr("Request error occured: %0").arg(m_replies.getProjekte->error()), {});
        goto end;
    }

    {
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(m_replies.getProjekte->readAll(), &error);
        if(error.error != QJsonParseError::NoError)
        {
            Q_EMIT getProjekteFinished(false, tr("Parsing JSON failed: %0").arg(error.errorString()), {});
            goto end;
        }

        if(!document.isObject())
        {
            Q_EMIT getProjekteFinished(false, tr("JSON document is not an object!"), {});
            goto end;
        }

        auto rootObj = document.object();

        if(!rootObj.contains(QStringLiteral("elements")))
        {
            Q_EMIT getProjekteFinished(false, tr("JSON does not contain elements!"), {});
            goto end;
        }

        auto elements = rootObj.value(QStringLiteral("elements"));

        if(!elements.isArray())
        {
            Q_EMIT getProjekteFinished(false, tr("elements is not an array!"), {});
            goto end;
        }

        auto elementsArr = elements.toArray();
        QVector<Projekt> projekte;

        for(const auto &val : elementsArr)
        {
            auto obj = val.toObject();

            projekte.append({
                                obj.value(QStringLiteral("label")).toString(),
                                obj.value(QStringLiteral("value")).toString()
                            });
        }

        Q_EMIT getProjekteFinished(true, QString(), projekte);
    }

    end:
    m_replies.getProjekte->deleteLater();
    m_replies.getProjekte = Q_NULLPTR;
}

void Zeiterfassung::getAuswertungRequest0Finished()
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
            this,                    &Zeiterfassung::getAuswertungRequest1Finished);
}

void Zeiterfassung::getAuswertungRequest1Finished()
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
