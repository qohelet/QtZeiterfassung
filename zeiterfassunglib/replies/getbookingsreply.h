#ifndef GETBOOKINGSREPLY_H
#define GETBOOKINGSREPLY_H

#include <memory>

#include <QNetworkReply>
#include <QVector>

#include "zeiterfassunglib_global.h"
#include "zeiterfassungreply.h"

class ZeiterfassungApi;

class ZEITERFASSUNGLIBSHARED_EXPORT GetBookingsReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    GetBookingsReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

    struct Booking
    {
        int id;
        QDate date;
        QTime time;
        QTime timespan;
        QString type;
        QString text;
    };

    const QVector<Booking> &bookings() const;

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    QVector<Booking> m_bookings;
};

#endif // GETBOOKINGSREPLY_H
