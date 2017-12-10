#ifndef USERINFOREPLY_H
#define USERINFOREPLY_H

#include "zeiterfassungreply.h"
#include "zeiterfassungapi.h"

class QNetworkReply;

class UserInfoReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    UserInfoReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);

    const ZeiterfassungApi::UserInfo &userInfo() const;

private Q_SLOTS:
    void requestFinished();

private:
    QNetworkReply *m_reply;
    ZeiterfassungApi::UserInfo m_userInfo;
};

#endif // USERINFOREPLY_H
