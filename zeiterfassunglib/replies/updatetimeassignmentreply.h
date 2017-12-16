#ifndef UPDATETIMEASSIGNMENTREPLY_H
#define UPDATETIMEASSIGNMENTREPLY_H

#include <memory>

#include <QNetworkReply>

#include "zeiterfassunglib_global.h"
#include "zeiterfassungreply.h"

class ZEITERFASSUNGLIBSHARED_EXPORT UpdateTimeAssignmentReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    UpdateTimeAssignmentReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

    int timeAssignmentId() const;

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    int m_timeAssignmentId;
};

#endif // UPDATETIMEASSIGNMENTREPLY_H
