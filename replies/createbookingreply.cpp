#include "createbookingreply.h"

#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

CreateBookingReply::CreateBookingReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung) :
    ZeiterfassungReply(zeiterfassung),
    m_reply(reply),
    m_bookingId(-1)
{
    connect(reply, &QNetworkReply::finished, this, &CreateBookingReply::requestFinished);
}

int CreateBookingReply::bookingId() const
{
    return m_bookingId;
}

void CreateBookingReply::requestFinished()
{
    if(m_replies.createBooking->error() != QNetworkReply::NoError)
    {
        setSuccess(false);
        setMessage(tr("Request error occured: %0").arg(m_replies.createBooking->error()));
        goto end;
    }

    {
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(m_replies.createBooking->readAll(), &error);
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
    m_replies.createBooking->deleteLater();
    m_replies.createBooking = Q_NULLPTR;

    Q_EMIT finished();
}
