#pragma once

#include <memory>

#include <QNetworkReply>
#include <QVector>

#include "zeiterfassungcorelib_global.h"
#include "zeiterfassungreply.h"

class ZeiterfassungApi;

class ZEITERFASSUNGCORELIBSHARED_EXPORT GetAbsencesReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    explicit GetAbsencesReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

    struct Absence
    {
        //TODO
    };

    const QVector<Absence> &getAbsences() const;

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    QVector<Absence> m_absences;
};
