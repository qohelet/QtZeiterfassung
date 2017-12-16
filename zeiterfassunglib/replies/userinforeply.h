#ifndef USERINFOREPLY_H
#define USERINFOREPLY_H

#include <memory>

#include <QNetworkReply>

#include "zeiterfassunglib_global.h"
#include "zeiterfassungreply.h"
#include "zeiterfassungapi.h"

class ZEITERFASSUNGLIBSHARED_EXPORT UserInfoReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    UserInfoReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

    const ZeiterfassungApi::UserInfo &userInfo() const;

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    ZeiterfassungApi::UserInfo m_userInfo;
};

#endif // USERINFOREPLY_H
