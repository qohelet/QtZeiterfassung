#ifndef CREATETIMEASSIGNMENTREPLY_H
#define CREATETIMEASSIGNMENTREPLY_H

#include <memory>

#include <QNetworkReply>

#include "zeiterfassungreply.h"

class CreateTimeAssignmentReply : public ZeiterfassungReply
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

#endif // CREATETIMEASSIGNMENTREPLY_H
