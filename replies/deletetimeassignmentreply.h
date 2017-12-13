#ifndef DELETETIMEASSIGNMENT_H
#define DELETETIMEASSIGNMENT_H

#include <memory>

#include <QNetworkReply>

#include "zeiterfassungreply.h"

class DeleteTimeAssignmentReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    DeleteTimeAssignmentReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
};

#endif // DELETETIMEASSIGNMENT_H
