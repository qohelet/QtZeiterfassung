#include "deletebookingreply.h"

DeleteBookingReply::DeleteBookingReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung) :
    ZeiterfassungReply(zeiterfassung),
    m_reply(std::move(reply))
{
    connect(m_reply.get(), &QNetworkReply::finished, this, &DeleteBookingReply::requestFinished);
}

void DeleteBookingReply::requestFinished()
{
    if(m_reply->error() != QNetworkReply::NoError)
    {
        setSuccess(false);
        setMessage(tr("Request error occured: %0").arg(m_reply->errorString()));
        goto end;
    }

    //should be empty, so nothing to check...

    setSuccess(true);

    end:
    m_reply = Q_NULLPTR;

    Q_EMIT finished();
}
