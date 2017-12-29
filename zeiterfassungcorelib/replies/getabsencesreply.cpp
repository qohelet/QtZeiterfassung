#include "getabsencesreply.h"

#include <QDebug>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

#include "zeiterfassungapi.h"

GetAbsencesReply::GetAbsencesReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung) :
    ZeiterfassungReply(zeiterfassung),
    m_reply(std::move(reply))
{
    connect(m_reply.get(), &QNetworkReply::finished, this, &GetAbsencesReply::requestFinished);
}

const QVector<GetAbsencesReply::Absence> &GetAbsencesReply::getAbsences() const
{
    return m_absences;
}

void GetAbsencesReply::requestFinished()
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
        m_absences.clear();
        m_absences.reserve(arr.count());
        for(const auto &val : arr)
        {
            auto obj = val.toObject();

            m_absences.append({
                obj.value(QStringLiteral("altRepresentative")).toInt(),
                obj.value(QStringLiteral("compositeId")).toString(),
                QDate::fromString(QString::number(obj.value(QStringLiteral("end")).toInt()), QStringLiteral("yyyyMMdd")),
                obj.value(QStringLiteral("hourCategory")).toString(),
                obj.value(QStringLiteral("openMarking")).toString(),
                obj.value(QStringLiteral("persNr")).toInt(),
                obj.value(QStringLiteral("representative")).toInt(),
                QDate::fromString(QString::number(obj.value(QStringLiteral("start")).toInt()), QStringLiteral("yyyyMMdd")),
                obj.value(QStringLiteral("text")).toString()
            });
        }
    }

    end:
    m_reply = Q_NULLPTR;

    Q_EMIT finished();
}
