#ifndef DELETEBOOKINGREPLY_H
#define DELETEBOOKINGREPLY_H

#include "zeiterfassungreply.h"

class QNetworkReply;

class DeleteBookingReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    DeleteBookingReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);
};

#endif // DELETEBOOKINGREPLY_H
