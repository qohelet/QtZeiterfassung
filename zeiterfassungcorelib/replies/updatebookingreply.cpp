#include "updatebookingreply.h"

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

UpdateBookingReply::UpdateBookingReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung) :
    ZeiterfassungReply(zeiterfassung),
    m_reply(std::move(reply)),
    m_bookingId(-1)
{
    connect(m_reply.get(), &QNetworkReply::finished, this, &UpdateBookingReply::requestFinished);
}

void UpdateBookingReply::requestFinished()
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

        if(!document.isObject())
        {
            setSuccess(false);
            setMessage(tr("JSON document is not an object!"));
            goto end;
        }

        auto obj = document.object();

        if(!obj.contains(QStringLiteral("bookingNr")))
        {
            setSuccess(false);
            setMessage(tr("JSON does not contain bookingNr!"));
            goto end;
        }

        setSuccess(true);
        m_bookingId = obj.value(QStringLiteral("bookingNr")).toInt();
    }

    end:
    m_reply = Q_NULLPTR;

    Q_EMIT finished();
}
