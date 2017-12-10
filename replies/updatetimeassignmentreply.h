#ifndef UPDATETIMEASSIGNMENTREPLY_H
#define UPDATETIMEASSIGNMENTREPLY_H

#include "zeiterfassungreply.h"

class QNetworkReply;

class UpdateTimeAssignmentReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    UpdateTimeAssignmentReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);
};

#endif // UPDATETIMEASSIGNMENTREPLY_H
