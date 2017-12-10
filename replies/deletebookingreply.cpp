#include "deletebookingreply.h"

#include <QNetworkReply>

DeleteBookingReply::DeleteBookingReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung) :
    ZeiterfassungReply(zeiterfassung),
    m_reply(reply)
{
    connect(reply, &QNetworkReply::finished, this, &DeleteBookingReply::requestFinished);
}

void DeleteBookingReply::requestFinished()
{
    if(m_reply->error() != QNetworkReply::NoError)
    {
        setSuccess(false);
        setMessage(tr("Request error occured: %0").arg(m_reply->error()));
        goto end;
    }

    //should be empty, so nothing to check...

    setSuccess(true);

    end:
    m_reply->deleteLater();
    m_reply = Q_NULLPTR;

    Q_EMIT finished();
}
