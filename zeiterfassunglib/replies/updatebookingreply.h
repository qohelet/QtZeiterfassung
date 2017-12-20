#pragma once

#include <memory>

#include <QNetworkReply>

#include "zeiterfassunglib_global.h"
#include "zeiterfassungreply.h"

class ZEITERFASSUNGLIBSHARED_EXPORT UpdateBookingReply : public ZeiterfassungReply
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
