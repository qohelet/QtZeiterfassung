#ifndef CREATEBOOKINGREPLY_H
#define CREATEBOOKINGREPLY_H

#include "zeiterfassungreply.h"

class QNetworkReply;

class CreateBookingReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    CreateBookingReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);
};

#endif // CREATEBOOKINGREPLY_H
