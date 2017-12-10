#ifndef DELETETIMEASSIGNMENT_H
#define DELETETIMEASSIGNMENT_H

#include "zeiterfassungreply.h"

class QNetworkReply;

class DeleteTimeAssignment : public ZeiterfassungReply
{
    Q_OBJECT

public:
    DeleteTimeAssignment(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);
};

#endif // DELETETIMEASSIGNMENT_H
