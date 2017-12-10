#ifndef CREATETIMEASSIGNMENTREPLY_H
#define CREATETIMEASSIGNMENTREPLY_H

#include "zeiterfassungreply.h"

class QNetworkReply;

class CreateTimeAssignmentReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    CreateTimeAssignmentReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);
};

#endif // CREATETIMEASSIGNMENTREPLY_H
