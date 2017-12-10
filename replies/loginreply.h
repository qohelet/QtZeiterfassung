#ifndef LOGINREPLY_H
#define LOGINREPLY_H

#include "zeiterfassungreply.h"

class QNetworkReply;

class LoginReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    LoginReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);

private Q_SLOTS:
    void requestFinished();

private:
    QNetworkReply *m_reply;
};

#endif // LOGINREPLY_H
