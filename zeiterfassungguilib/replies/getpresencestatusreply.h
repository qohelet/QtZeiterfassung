#pragma once

#include <memory>

#include <QNetworkReply>

#include "zeiterfassungguilib_global.h"
#include "zeiterfassungreply.h"

class ZeiterfassungApi;

class ZEITERFASSUNGGUILIBSHARED_EXPORT GetPresenceStatusReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    explicit GetPresenceStatusReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

    struct PresenceStatus
    {
        int userId;
        QString firstName;
        QString lastName;
        QString presence;
    };

    const QVector<PresenceStatus> &presenceStatuses() const;

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    QVector<PresenceStatus> m_presenceStatuses;
};
