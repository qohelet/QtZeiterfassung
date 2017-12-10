#ifndef GETTIMEASSIGNMENTSREPLY_H
#define GETTIMEASSIGNMENTSREPLY_H

#include <QVector>

#include "zeiterfassungreply.h"
#include "zeiterfassungapi.h"

class QNetworkReply;

class GetTimeAssignmentsReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    GetTimeAssignmentsReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);

    const QVector<ZeiterfassungApi::TimeAssignment> &timeAssignments() const;

private Q_SLOTS:
    void requestFinished();

private:
    QNetworkReply *m_reply;
    QVector<ZeiterfassungApi::TimeAssignment> m_timeAssignments;
};

#endif // GETTIMEASSIGNMENTSREPLY_H
