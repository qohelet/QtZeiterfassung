#ifndef GETTIMEASSIGNMENTSREPLY_H
#define GETTIMEASSIGNMENTSREPLY_H

#include <memory>

#include <QNetworkReply>
#include <QVector>

#include "zeiterfassungreply.h"
#include "zeiterfassungapi.h"

class GetTimeAssignmentsReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    GetTimeAssignmentsReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

    const QVector<ZeiterfassungApi::TimeAssignment> &timeAssignments() const;

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    QVector<ZeiterfassungApi::TimeAssignment> m_timeAssignments;
};

#endif // GETTIMEASSIGNMENTSREPLY_H
