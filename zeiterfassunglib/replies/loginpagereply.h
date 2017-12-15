#ifndef LOGINPAGEREPLY_H
#define LOGINPAGEREPLY_H

#include <memory>

#include <QNetworkReply>

#include "zeiterfassunglib_global.h"
#include "zeiterfassungreply.h"

class ZEITERFASSUNGLIBSHARED_EXPORT LoginPageReply : public ZeiterfassungReply
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
