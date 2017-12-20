#pragma once

#include <memory>

#include <QNetworkReply>

#include "zeiterfassungguilib_global.h"
#include "zeiterfassungreply.h"

class ZEITERFASSUNGGUILIBSHARED_EXPORT DeleteBookingReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    DeleteBookingReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
};
