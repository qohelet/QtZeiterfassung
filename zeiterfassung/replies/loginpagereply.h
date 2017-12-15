#ifndef LOGINPAGEREPLY_H
#define LOGINPAGEREPLY_H

#include <memory>

#include <QNetworkReply>

#include "zeiterfassungreply.h"

class LoginPageReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    LoginPageReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
};

#endif // LOGINPAGEREPLY_H
