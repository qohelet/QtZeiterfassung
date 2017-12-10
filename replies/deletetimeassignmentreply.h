#ifndef DELETETIMEASSIGNMENT_H
#define DELETETIMEASSIGNMENT_H

#include "zeiterfassungreply.h"

class QNetworkReply;

class DeleteTimeAssignmentReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    DeleteTimeAssignmentReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);

private Q_SLOTS:
    void requestFinished();

private:
    QNetworkReply *m_reply;
};

#endif // DELETETIMEASSIGNMENT_H
