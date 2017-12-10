#ifndef GETBOOKINGSREPLY_H
#define GETBOOKINGSREPLY_H

#include "zeiterfassungreply.h"

class QNetworkReply;

class GetBookingsReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    GetBookingsReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);
};

#endif // GETBOOKINGSREPLY_H
