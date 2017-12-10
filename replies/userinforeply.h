#ifndef USERINFOREPLY_H
#define USERINFOREPLY_H

#include "zeiterfassungreply.h"

class QNetworkReply;

class UserInfoReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    UserInfoReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);
};

#endif // USERINFOREPLY_H
