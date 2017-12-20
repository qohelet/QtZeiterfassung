#pragma once

#include <memory>

#include <QNetworkReply>

#include "zeiterfassungguilib_global.h"
#include "zeiterfassungreply.h"

class ZEITERFASSUNGGUILIBSHARED_EXPORT UpdateBookingReply : public ZeiterfassungReply
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
