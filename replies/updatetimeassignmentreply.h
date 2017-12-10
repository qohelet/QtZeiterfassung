#ifndef UPDATETIMEASSIGNMENTREPLY_H
#define UPDATETIMEASSIGNMENTREPLY_H

#include "zeiterfassungreply.h"

class QNetworkReply;

class UpdateTimeAssignmentReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    UpdateTimeAssignmentReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);

    int timeAssignmentId() const;

private Q_SLOTS:
    void requestFinished();

private:
    QNetworkReply *m_reply;
    int m_timeAssignmentId;
};

#endif // UPDATETIMEASSIGNMENTREPLY_H
