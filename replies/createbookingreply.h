#ifndef CREATEBOOKINGREPLY_H
#define CREATEBOOKINGREPLY_H

#include "zeiterfassungreply.h"

class QNetworkReply;

class CreateBookingReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    CreateBookingReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);

    int bookingId() const;

private Q_SLOTS:
    void requestFinished();

private:
    QNetworkReply *m_reply;
    int m_bookingId;
};

#endif // CREATEBOOKINGREPLY_H
