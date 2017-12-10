#ifndef UPDATEBOOKINGREPLY_H
#define UPDATEBOOKINGREPLY_H

#include "zeiterfassungreply.h"

class QNetworkReply;

class UpdateBookingReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    UpdateBookingReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);

private Q_SLOTS:
    void requestFinished();

private:
    QNetworkReply *m_reply;
    int m_bookingId;
};

#endif // UPDATEBOOKINGREPLY_H
