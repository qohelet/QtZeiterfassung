#include "getprojectsreply.h"

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include "zeiterfassungapi.h"

GetProjectsReply::GetProjectsReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung) :
    ZeiterfassungReply(zeiterfassung),
    m_reply(std::move(reply))
{
    connect(m_reply.get(), &QNetworkReply::finished, this, &GetProjectsReply::requestFinished);
}

const QVector<GetProjectsReply::Project> &GetProjectsReply::projects() const
{
    return m_projects;
}

void GetProjectsReply::requestFinished()
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

        auto rootObj = document.object();

        if(!rootObj.contains(QStringLiteral("elements")))
        {
            setSuccess(false);
            setMessage(tr("JSON does not contain elements!"));
            goto end;
        }

        auto elements = rootObj.value(QStringLiteral("elements"));

        if(!elements.isArray())
        {
            setSuccess(false);
            setMessage(tr("elements is not an array!"));
            goto end;
        }

        auto elementsArr = elements.toArray();

        setSuccess(true);
        m_projects.clear();
        m_projects.reserve(elementsArr.count());
        for(const auto &val : elementsArr)
        {
            auto obj = val.toObject();

            m_projects.append({
                obj.value(QStringLiteral("label")).toString(),
                obj.value(QStringLiteral("value")).toString()
            });
        }
    }

    end:
    m_reply = Q_NULLPTR;

    Q_EMIT finished();
}
