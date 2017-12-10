#include "getbookingsreply.h"

#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

GetBookingsReply::GetBookingsReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung) :
    ZeiterfassungReply(zeiterfassung),
    m_reply(reply)
{
    connect(reply, &QNetworkReply::finished, this, &GetBookingsReply::requestFinished);
}

const QVector<ZeiterfassungApi::Booking> &GetBookingsReply::bookings() const
{
    return m_bookings;
}

void GetBookingsReply::requestFinished()
{
    if(m_reply->error() != QNetworkReply::NoError)
    {
        setSuccess(false);
        setMessage(tr("Request error occured: %0").arg(m_reply->error()));
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
        m_bookings.clear();
        for(const auto &val : arr)
        {
            auto obj = val.toObject();

            m_bookings.append({
                obj.value(QStringLiteral("bookingNr")).toInt(),
                QDate::fromString(QString::number(obj.value(QStringLiteral("bookingDate")).toInt()), QStringLiteral("yyyyMMdd")),
                QTime::fromString(QString("%0").arg(obj.value(QStringLiteral("bookingTime")).toInt(), 6, 10, QChar('0')), QStringLiteral("HHmmss")),
                QTime::fromString(QString("%0").arg(obj.value(QStringLiteral("bookingTimespan")).toInt(), 6, 10, QChar('0')), QStringLiteral("HHmmss")),
                obj.value(QStringLiteral("bookingType")).toString(),
                obj.value(QStringLiteral("text")).toString()
            });
        }
    }

    end:
    m_reply->deleteLater();
    m_reply = Q_NULLPTR;

    Q_EMIT finished();
}
