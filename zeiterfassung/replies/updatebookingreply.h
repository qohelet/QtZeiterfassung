#ifndef UPDATEBOOKINGREPLY_H
#define UPDATEBOOKINGREPLY_H

#include <memory>

#include <QNetworkReply>

#include "zeiterfassungreply.h"

class UpdateBookingReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    UpdateBookingReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    int m_bookingId;
};

#endif // UPDATEBOOKINGREPLY_H
