#include "loginpagereply.h"

#include <QNetworkReply>

LoginPageReply::LoginPageReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung) :
    ZeiterfassungReply(zeiterfassung),
    m_reply(std::move(reply))
{
    connect(m_reply.get(), &QNetworkReply::finished, this, &LoginPageReply::requestFinished);
}

void LoginPageReply::requestFinished()
{
    if(m_reply->error() != QNetworkReply::NoError)
    {
        setSuccess(false);
        setMessage(tr("Request error occured: %0").arg(m_reply->errorString()));
        goto end;
    }

    if(!m_reply->readAll().contains(QByteArrayLiteral("evoApps Anmeldung")))
    {
        setSuccess(false);
        setMessage(tr("Could not find necessary keywords in login page!"));
        goto end;
    }

    setSuccess(true);

    end:
    m_reply = Q_NULLPTR;

    Q_EMIT finished();
}
