#ifndef GETUSERINFOREPLY_H
#define GETUSERINFOREPLY_H

#include <memory>

#include <QNetworkReply>

#include "zeiterfassunglib_global.h"
#include "zeiterfassungreply.h"

class ZeiterfassungApi;

class ZEITERFASSUNGLIBSHARED_EXPORT GetUserInfoReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    GetUserInfoReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

    struct UserInfo
    {
        int userId;
        QString email;
        QString longUsername;
        QString text;
        QString username;
    };

    const UserInfo &userInfo() const;

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    UserInfo m_userInfo;
};

#endif // GETUSERINFOREPLY_H
