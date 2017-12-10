#ifndef GETAUSWERTUNGREPLY_H
#define GETAUSWERTUNGREPLY_H

#include <QByteArray>

#include "zeiterfassungreply.h"

class QNetworkReply;

class GetAuswertungReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    GetAuswertungReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);

    const QByteArray &auswertung() const;

private Q_SLOTS:
    void request0Finished();
    void request1Finished();

private:
    QNetworkReply *m_reply;
    QByteArray m_auswertung;
};

#endif // GETAUSWERTUNGREPLY_H
