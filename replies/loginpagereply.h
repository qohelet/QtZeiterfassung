#ifndef LOGINPAGEREPLY_H
#define LOGINPAGEREPLY_H

#include "zeiterfassungreply.h"

class QNetworkReply;

class LoginPageReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    LoginPageReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);
};

#endif // LOGINPAGEREPLY_H
