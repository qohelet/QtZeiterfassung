#ifndef GETBOOKINGSREPLY_H
#define GETBOOKINGSREPLY_H

#include <QVector>

#include "zeiterfassungreply.h"
#include "zeiterfassungapi.h"

class QNetworkReply;

class GetBookingsReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    GetBookingsReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);

    const QVector<ZeiterfassungApi::Booking> &bookings() const;

private Q_SLOTS:
    void requestFinished();

private:
    QNetworkReply *m_reply;
    QVector<ZeiterfassungApi::Booking> m_bookings;
};

#endif // GETBOOKINGSREPLY_H
