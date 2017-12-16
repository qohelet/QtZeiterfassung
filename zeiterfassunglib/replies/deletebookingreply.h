#ifndef DELETEBOOKINGREPLY_H
#define DELETEBOOKINGREPLY_H

#include <memory>

#include <QNetworkReply>

#include "zeiterfassunglib_global.h"
#include "zeiterfassungreply.h"

class ZEITERFASSUNGLIBSHARED_EXPORT DeleteBookingReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    DeleteBookingReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
};

#endif // DELETEBOOKINGREPLY_H
