#include "getreportreply.h"

#include "zeiterfassungapi.h"

GetReportReply::GetReportReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung) :
    ZeiterfassungReply(zeiterfassung),
    m_reply(std::move(reply))
{
    connect(m_reply.get(), &QNetworkReply::finished, this, &GetReportReply::request0Finished);
}

const QByteArray &GetReportReply::content() const
{
    return m_content;
}

void GetReportReply::request0Finished()
{
    if(m_reply->error() != QNetworkReply::NoError)
    {
        setSuccess(false);
        setMessage(tr("Request error occured: %0").arg(m_reply->errorString()));
        m_reply = Q_NULLPTR;
        Q_EMIT finished();
        return;
    }

    QUrl url(zeiterfassung()->url());
    url.setPath(QString(m_reply->readAll()));

    m_reply = std::unique_ptr<QNetworkReply>(zeiterfassung()->manager()->get(QNetworkRequest(url)));
    connect(m_reply.get(), &QNetworkReply::finished, this, &GetReportReply::request1Finished);
}

void GetReportReply::request1Finished()
{
    if(m_reply->error() != QNetworkReply::NoError)
    {
        setSuccess(false);
        setMessage(tr("Request error occured: %0").arg(m_reply->errorString()));
        goto end;
    }

    setSuccess(true);
    m_content = m_reply->readAll();

    end:
    m_reply = Q_NULLPTR;

    Q_EMIT finished();
}
