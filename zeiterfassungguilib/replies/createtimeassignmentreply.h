#pragma once

#include <memory>

#include <QNetworkReply>

#include "zeiterfassungguilib_global.h"
#include "zeiterfassungreply.h"

class ZEITERFASSUNGGUILIBSHARED_EXPORT CreateTimeAssignmentReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    CreateTimeAssignmentReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

    int timeAssignmentId() const;

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    int m_timeAssignmentId;
};
