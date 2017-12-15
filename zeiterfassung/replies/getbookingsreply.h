#ifndef GETBOOKINGSREPLY_H
#define GETBOOKINGSREPLY_H

#include <memory>

#include <QNetworkReply>
#include <QVector>

#include "zeiterfassungreply.h"
#include "zeiterfassungapi.h"

class GetBookingsReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    GetBookingsReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

    const QVector<ZeiterfassungApi::Booking> &bookings() const;

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    QVector<ZeiterfassungApi::Booking> m_bookings;
};

#endif // GETBOOKINGSREPLY_H
