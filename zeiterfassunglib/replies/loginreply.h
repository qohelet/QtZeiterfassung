#ifndef LOGINREPLY_H
#define LOGINREPLY_H

#include <memory>

#include <QNetworkReply>

#include "zeiterfassunglib_global.h"
#include "zeiterfassungreply.h"

class ZEITERFASSUNGLIBSHARED_EXPORT LoginReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    LoginReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
};

#endif // LOGINREPLY_H