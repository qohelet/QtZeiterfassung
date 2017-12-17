#ifndef CREATEBOOKINGREPLY_H
#define CREATEBOOKINGREPLY_H

#include <memory>

#include <QNetworkReply>

#include "zeiterfassunglib_global.h"
#include "zeiterfassungreply.h"

class ZEITERFASSUNGLIBSHARED_EXPORT CreateBookingReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    CreateBookingReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

    int bookingId() const;

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    int m_bookingId;
};

#endif // CREATEBOOKINGREPLY_H
