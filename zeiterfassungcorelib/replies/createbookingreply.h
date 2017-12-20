#pragma once

#include <memory>

#include <QNetworkReply>

#include "zeiterfassungcorelib_global.h"
#include "zeiterfassungreply.h"

class ZEITERFASSUNGCORELIBSHARED_EXPORT CreateBookingReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    CreateBookingReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
};
