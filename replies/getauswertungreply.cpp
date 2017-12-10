#include "getauswertungreply.h"

#include <QNetworkReply>

#include "zeiterfassungapi.h"

GetAuswertungReply::GetAuswertungReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung) :
    ZeiterfassungReply(zeiterfassung),
    m_reply(reply)
{
    connect(reply, &QNetworkReply::finished, this, &GetAuswertungReply::request0Finished);
}

const QByteArray &GetAuswertungReply::auswertung() const
{
    return m_auswertung;
}

void GetAuswertungReply::request0Finished()
{
    if(m_reply->error() != QNetworkReply::NoError)
    {
        setSuccess(false);
        setMessage(tr("Request error occured: %0").arg(m_reply->error()));
        m_reply->deleteLater();
        m_reply = Q_NULLPTR;
        Q_EMIT finished();
        return;
    }

    QUrl url(zeiterfassung()->url());
    url.setPath(QString(m_reply->readAll()));

    m_reply->deleteLater();

    m_reply = zeiterfassung()->manager()->get(QNetworkRequest(url));
    connect(m_reply, &QNetworkReply::finished, this, &GetAuswertungReply::request1Finished);
}

void GetAuswertungReply::request1Finished()
{
    if(m_reply->error() != QNetworkReply::NoError)
    {
        setSuccess(false);
        setMessage(tr("Request error occured: %0").arg(m_reply->error()));
        goto end;
    }

    setSuccess(true);
    m_auswertung = m_reply->readAll();

    end:
    m_reply->deleteLater();
    m_reply = Q_NULLPTR;

    Q_EMIT finished();
}
