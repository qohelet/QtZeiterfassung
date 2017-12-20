#include "getuserinforeply.h"

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include "zeiterfassungapi.h"

GetUserInfoReply::GetUserInfoReply(std::unique_ptr<QNetworkReply> &&reply0, std::unique_ptr<QNetworkReply> &&reply1,
                                   ZeiterfassungApi *zeiterfassung) :
    ZeiterfassungReply(zeiterfassung),
    m_reply0(std::move(reply0)),
    m_reply1(std::move(reply1))
{
    Q_ASSERT(m_reply0 != Q_NULLPTR);
    Q_ASSERT(m_reply1 != Q_NULLPTR);

    connect(m_reply0.get(), &QNetworkReply::finished, this, &GetUserInfoReply::request0Finished);
    connect(m_reply1.get(), &QNetworkReply::finished, this, &GetUserInfoReply::request1Finished);
}

const GetUserInfoReply::UserInfo &GetUserInfoReply::userInfo() const
{
    return m_userInfo;
}

void GetUserInfoReply::request0Finished()
{
    if(m_reply0->error() != QNetworkReply::NoError)
    {
        setSuccess(false);
        setMessage(tr("Request 0 error occured: %0").arg(m_reply0->error()));
        m_reply1 = Q_NULLPTR;
        goto end;
    }

    {
        QJsonParseError error;
        auto document = QJsonDocument::fromJson(m_reply0->readAll(), &error);
        if(error.error != QJsonParseError::NoError)
        {
            setSuccess(false);
            setMessage(tr("Parsing JSON 0 failed: %0").arg(error.errorString()));
            m_reply1 = Q_NULLPTR;
            goto end;
        }

        if(!document.isObject())
        {
            setSuccess(false);
            setMessage(tr("JSON document 0 is not an object!"));
            m_reply1 = Q_NULLPTR;
            goto end;
        }

        auto rootObj = document.object();

        if(!rootObj.contains(QStringLiteral("evoAppsUser")))
        {
            setSuccess(false);
            setMessage(tr("JSON 0 does not contain evoAppsUser!"));
            m_reply1 = Q_NULLPTR;
            goto end;
        }

        auto evoAppsUser = rootObj.value(QStringLiteral("evoAppsUser"));

        if(!evoAppsUser.isObject())
        {
            setSuccess(false);
            setMessage(tr("evoAppsUser is not an object!"));
            m_reply1 = Q_NULLPTR;
            goto end;
        }

        auto evoAppsUserObj = evoAppsUser.toObject();

        if(!m_reply1)
            setSuccess(true);

        m_userInfo.userId = evoAppsUserObj.value(QStringLiteral("persNr")).toInt();
        m_userInfo.email = evoAppsUserObj.value(QStringLiteral("email")).toString();
        m_userInfo.longUsername = evoAppsUserObj.value(QStringLiteral("longUsername")).toString();
        m_userInfo.text = evoAppsUserObj.value(QStringLiteral("text")).toString();
        m_userInfo.username = evoAppsUserObj.value(QStringLiteral("username")).toString();
    }

    end:
    m_reply0 = Q_NULLPTR;

    if(!m_reply1)
        Q_EMIT finished();
}

void GetUserInfoReply::request1Finished()
{
    if(m_reply1->error() != QNetworkReply::NoError)
    {
        setSuccess(false);
        setMessage(tr("Request 1 error occured: %0").arg(m_reply0->error()));
        m_reply0 = Q_NULLPTR;
        goto end;
    }

    {
        QJsonParseError error;
        auto document = QJsonDocument::fromJson(m_reply1->readAll(), &error);
        if(error.error != QJsonParseError::NoError)
        {
            setSuccess(false);
            setMessage(tr("Parsing JSON 1 failed: %0").arg(error.errorString()));
            m_reply0 = Q_NULLPTR;
            goto end;
        }

        if(!document.isArray())
        {
            setSuccess(false);
            setMessage(tr("JSON document 1 is not an array!"));
            m_reply0 = Q_NULLPTR;
            goto end;
        }

        auto arr = document.array();

        if(arr.isEmpty())
        {
            setSuccess(false);
            setMessage(tr("JSON array 1 is empty!"));
            m_reply0 = Q_NULLPTR;
            goto end;
        }

        auto first = arr.first();

        if(!first.isObject())
        {
            setSuccess(false);
            setMessage(tr("JSON array value is not an object!"));
            m_reply0 = Q_NULLPTR;
            goto end;
        }

        auto obj = first.toObject();

        if(!m_reply0)
            setSuccess(true);

        m_userInfo.gemeinde = obj.value(QStringLiteral("gemeinde")).toString();
        m_userInfo.ort = obj.value(QStringLiteral("ort")).toString();
        m_userInfo.angFrom = QDate::fromString(QString::number(obj.value(QStringLiteral("angFrom")).toInt()), QStringLiteral("yyyyMMdd"));
        m_userInfo.angTill = QDate::fromString(QString::number(obj.value(QStringLiteral("angTill")).toInt()), QStringLiteral("yyyyMMdd"));
        m_userInfo.gebOrt = obj.value(QStringLiteral("gebOrt")).toString();
        m_userInfo.plz = obj.value(QStringLiteral("plz")).toString();
        m_userInfo.religion = obj.value(QStringLiteral("religion")).toString();
        m_userInfo.bereich = obj.value(QStringLiteral("bereich")).toString();
        m_userInfo.verwendgr = obj.value(QStringLiteral("verwendgr")).toString();
        m_userInfo.taetig = obj.value(QStringLiteral("taetig")).toString();
        m_userInfo.arbverh = obj.value(QStringLiteral("arbverh")).toString();
        m_userInfo.betriebsnr = obj.value(QStringLiteral("betriebsnr")).toString();
    }

    end:
    m_reply1 = Q_NULLPTR;

    if(!m_reply0)
        Q_EMIT finished();
}
