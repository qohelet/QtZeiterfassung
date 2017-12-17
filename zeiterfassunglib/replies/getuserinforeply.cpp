#include "getuserinforeply.h"

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include "zeiterfassungapi.h"

GetUserInfoReply::GetUserInfoReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung) :
    ZeiterfassungReply(zeiterfassung),
    m_reply(std::move(reply))
{
    connect(m_reply.get(), &QNetworkReply::finished, this, &GetUserInfoReply::requestFinished);
}

const GetUserInfoReply::UserInfo &GetUserInfoReply::userInfo() const
{
    return m_userInfo;
}

void GetUserInfoReply::requestFinished()
{
    if(m_reply->error() != QNetworkReply::NoError)
    {
        setSuccess(false);
        setMessage(tr("Request error occured: %0").arg(m_reply->error()));
        goto end;
    }

    {
        QJsonParseError error;
        auto document = QJsonDocument::fromJson(m_reply->readAll(), &error);
        if(error.error != QJsonParseError::NoError)
        {
            setSuccess(false);
            setMessage(tr("Parsing JSON failed: %0").arg(error.errorString()));
            goto end;
        }

        if(!document.isObject())
        {
            setSuccess(false);
            setMessage(tr("JSON document is not an object!"));
            goto end;
        }

        auto rootObj = document.object();

        if(!rootObj.contains(QStringLiteral("evoAppsUser")))
        {
            setSuccess(false);
            setMessage(tr("JSON does not contain evoAppsUser!"));
            goto end;
        }

        auto evoAppsUser = rootObj.value(QStringLiteral("evoAppsUser"));

        if(!evoAppsUser.isObject())
        {
            setSuccess(false);
            setMessage(tr("evoAppsUser is not an object!"));
            goto end;
        }

        auto evoAppsUserObj = evoAppsUser.toObject();

        setSuccess(true);
        m_userInfo = {
            evoAppsUserObj.value(QStringLiteral("persNr")).toInt(),
            evoAppsUserObj.value(QStringLiteral("email")).toString(),
            evoAppsUserObj.value(QStringLiteral("longUsername")).toString(),
            evoAppsUserObj.value(QStringLiteral("text")).toString(),
            evoAppsUserObj.value(QStringLiteral("username")).toString()
        };
    }

    end:
    m_reply = Q_NULLPTR;

    Q_EMIT finished();
}
