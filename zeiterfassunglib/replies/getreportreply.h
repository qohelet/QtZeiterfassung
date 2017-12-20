#pragma once

#include <memory>

#include <QNetworkReply>
#include <QByteArray>

#include "zeiterfassunglib_global.h"
#include "zeiterfassungreply.h"

class ZEITERFASSUNGLIBSHARED_EXPORT GetReportReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    GetReportReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

    const QByteArray &content() const;

private Q_SLOTS:
    void request0Finished();
    void request1Finished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    QByteArray m_content;
};
