#ifndef GETAUSWERTUNGREPLY_H
#define GETAUSWERTUNGREPLY_H

#include <memory>

#include <QNetworkReply>
#include <QByteArray>

#include "zeiterfassunglib_global.h"
#include "zeiterfassungreply.h"

class ZEITERFASSUNGLIBSHARED_EXPORT GetAuswertungReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    GetAuswertungReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

    const QByteArray &auswertung() const;

private Q_SLOTS:
    void request0Finished();
    void request1Finished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    QByteArray m_auswertung;
};

#endif // GETAUSWERTUNGREPLY_H
