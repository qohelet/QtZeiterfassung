#ifndef GETPROJECTSREPLY_H
#define GETPROJECTSREPLY_H

#include "zeiterfassungreply.h"

class QNetworkReply;

class GetProjectsReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    GetProjectsReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);
};

#endif // GETPROJECTSREPLY_H
