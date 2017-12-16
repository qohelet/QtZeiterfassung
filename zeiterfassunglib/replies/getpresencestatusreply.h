#ifndef GETPRESENCESTATUSREPLY_H
#define GETPRESENCESTATUSREPLY_H

#include <memory>

#include <QNetworkReply>

#include "zeiterfassunglib_global.h"
#include "zeiterfassungreply.h"
#include "zeiterfassungapi.h"

class ZEITERFASSUNGLIBSHARED_EXPORT GetPresenceStatusReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    explicit GetPresenceStatusReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

    const QVector<ZeiterfassungApi::PresenceStatus> &presenceStatuses() const;

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    QVector<ZeiterfassungApi::PresenceStatus> m_presenceStatuses;
};

#endif // GETPRESENCESTATUSREPLY_H
