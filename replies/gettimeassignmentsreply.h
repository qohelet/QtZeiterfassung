#ifndef GETTIMEASSIGNMENTSREPLY_H
#define GETTIMEASSIGNMENTSREPLY_H

#include "zeiterfassungreply.h"

class QNetworkReply;

class GetTimeAssignmentsReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    GetTimeAssignmentsReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);
};

#endif // GETTIMEASSIGNMENTSREPLY_H
