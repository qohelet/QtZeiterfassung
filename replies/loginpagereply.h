#ifndef LOGINPAGEREPLY_H
#define LOGINPAGEREPLY_H

#include "zeiterfassungreply.h"

class QNetworkReply;

class LoginPageReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    LoginPageReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);

private Q_SLOTS:
    void requestFinished();

private:
    QNetworkReply *m_reply;
};

#endif // LOGINPAGEREPLY_H
