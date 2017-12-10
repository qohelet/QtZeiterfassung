#include "deletetimeassignmentreply.h"

#include <QNetworkReply>

DeleteTimeAssignmentReply::DeleteTimeAssignmentReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung) :
    ZeiterfassungReply(zeiterfassung),
    m_reply(reply)
{
    connect(reply, &QNetworkReply::finished, this, &DeleteTimeAssignmentReply::requestFinished);
}

void DeleteTimeAssignmentReply::requestFinished()
{
    if(m_reply->error() != QNetworkReply::NoError)
    {
        setSuccess(false);
        setMessage(tr("Request error occured: %0").arg(m_reply->error()));
        goto end;
    }

    //only contains deleted id, so nothing to check here

    setSuccess(true);

    end:
    m_reply->deleteLater();
    m_reply = Q_NULLPTR;

    Q_EMIT finished();
}
