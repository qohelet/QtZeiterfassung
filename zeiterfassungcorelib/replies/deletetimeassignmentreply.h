#pragma once

#include <memory>

#include <QNetworkReply>

#include "zeiterfassungcorelib_global.h"
#include "zeiterfassungreply.h"

class ZEITERFASSUNGCORELIBSHARED_EXPORT DeleteTimeAssignmentReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    explicit DeleteTimeAssignmentReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
};
