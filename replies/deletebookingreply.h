#ifndef DELETEBOOKINGREPLY_H
#define DELETEBOOKINGREPLY_H

#include "zeiterfassungreply.h"

class QNetworkReply;

class DeleteBookingReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    DeleteBookingReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);

private Q_SLOTS:
    void requestFinished();

private:
    QNetworkReply *m_reply;
};

#endif // DELETEBOOKINGREPLY_H
