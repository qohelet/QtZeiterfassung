#include "loginreply.h"

#include <QNetworkReply>

LoginReply::LoginReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung) :
    ZeiterfassungReply(zeiterfassung),
    m_reply(reply)
{
    connect(reply, &QNetworkReply::finished, this, &LoginReply::requestFinished);
}

void LoginReply::requestFinished()
{
    if(m_reply->error() != QNetworkReply::NoError)
    {
        setSuccess(false);
        setMessage(tr("Request error occured: %0").arg(m_reply->error()));
        goto end;
    }

    if(!m_reply->hasRawHeader(QByteArrayLiteral("Location")))
    {
        setSuccess(false);
        setMessage(tr("Request did not contain a Location header."));
        goto end;
    }

    {
        auto location = m_reply->rawHeader(QByteArrayLiteral("Location"));

        if(location == QByteArrayLiteral("/evoApps/pages/home.jsp"))
        {
            setSuccess(true);
            goto end;
        }
        else if(location == QByteArrayLiteral("/evoApps/pages/login.jsp?error=user"))
        {
            setSuccess(false);
            setMessage(tr("Authentication failure. Please check username and password."));
            goto end;
        }
        else
        {
            setSuccess(false);
            setMessage(tr("An unknown authentication failure occured. Redirected to: %0").arg(QString(location)));
            goto end;
        }
    }

    end:
    m_reply->deleteLater();
    m_reply = Q_NULLPTR;

    Q_EMIT finished();
}
