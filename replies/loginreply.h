#ifndef LOGINREPLY_H
#define LOGINREPLY_H

#include "zeiterfassungreply.h"

class QNetworkReply;

class LoginReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    LoginReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);
};

#endif // LOGINREPLY_H
