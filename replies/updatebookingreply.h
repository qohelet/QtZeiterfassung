#ifndef UPDATEBOOKINGREPLY_H
#define UPDATEBOOKINGREPLY_H

#include "zeiterfassungreply.h"

class QNetworkReply;

class UpdateBookingReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    UpdateBookingReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);
};

#endif // UPDATEBOOKINGREPLY_H
