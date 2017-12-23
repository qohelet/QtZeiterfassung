#include "createbookingreply.h"

CreateBookingReply::CreateBookingReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung) :
    ZeiterfassungReply(zeiterfassung),
    m_reply(std::move(reply))
{
    connect(m_reply.get(), &QNetworkReply::finished, this, &CreateBookingReply::requestFinished);
}

void CreateBookingReply::requestFinished()
{
    if(m_reply->error() != QNetworkReply::NoError)
    {
        setSuccess(false);
        setMessage(tr("Request error occured: %0").arg(m_reply->errorString()));
        goto end;
    }

    // empty response so nothing to check
    setSuccess(true);

    end:
    m_reply = Q_NULLPTR;

    Q_EMIT finished();
}
