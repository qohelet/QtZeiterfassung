#include "getpresencestatusreply.h"

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

#include "zeiterfassungapi.h"

GetPresenceStatusReply::GetPresenceStatusReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung) :
    ZeiterfassungReply(zeiterfassung),
    m_reply(std::move(reply))
{
    connect(m_reply.get(), &QNetworkReply::finished, this, &GetPresenceStatusReply::requestFinished);
}

const QVector<GetPresenceStatusReply::PresenceStatus> &GetPresenceStatusReply::presenceStatuses() const
{
    return m_presenceStatuses;
}

void GetPresenceStatusReply::requestFinished()
{
    if(m_reply->error() != QNetworkReply::NoError)
    {
        setSuccess(false);
        setMessage(tr("Request error occured: %0").arg(m_reply->errorString()));
        goto end;
    }

    {
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(m_reply->readAll(), &error);
        if(error.error != QJsonParseError::NoError)
        {
            setSuccess(false);
            setMessage(tr("Parsing JSON failed: %0").arg(error.errorString()));
            goto end;
        }

        if(!document.isArray())
        {
            setSuccess(false);
            setMessage(tr("JSON document is not an array!"));
            goto end;
        }

        auto arr = document.array();

        setSuccess(true);
        m_presenceStatuses.clear();
        m_presenceStatuses.reserve(arr.count());
        for(const auto &val : arr)
        {
            auto obj = val.toObject();

            m_presenceStatuses.append({
                obj.value(QStringLiteral("persNr")).toInt(),
                obj.value(QStringLiteral("firstName")).toString(),
                obj.value(QStringLiteral("lastName")).toString(),
                obj.value(QStringLiteral("presence")).toString()
            });
        }
    }

    end:
    m_reply = Q_NULLPTR;

    Q_EMIT finished();
}
