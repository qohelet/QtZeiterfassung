#ifndef GETAUSWERTUNGREPLY_H
#define GETAUSWERTUNGREPLY_H

#include "zeiterfassungreply.h"

class QNetworkReply;

class GetAuswertungReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    GetAuswertungReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);
};

#endif // GETAUSWERTUNGREPLY_H
