#ifndef CREATETIMEASSIGNMENTREPLY_H
#define CREATETIMEASSIGNMENTREPLY_H

#include "zeiterfassungreply.h"

class QNetworkReply;

class CreateTimeAssignmentReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    CreateTimeAssignmentReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);

    int timeAssignmentId() const;

private Q_SLOTS:
    void requestFinished();

private:
    QNetworkReply *m_reply;
    int m_timeAssignmentId;
};

#endif // CREATETIMEASSIGNMENTREPLY_H
