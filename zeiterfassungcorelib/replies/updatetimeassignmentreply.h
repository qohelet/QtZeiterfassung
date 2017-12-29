#pragma once

#include <memory>

#include <QNetworkReply>

#include "zeiterfassungcorelib_global.h"
#include "zeiterfassungreply.h"

class ZEITERFASSUNGCORELIBSHARED_EXPORT UpdateTimeAssignmentReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    explicit UpdateTimeAssignmentReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

    int timeAssignmentId() const;

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    int m_timeAssignmentId;
};
