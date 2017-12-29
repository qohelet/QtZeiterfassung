#pragma once

#include <memory>

#include <QString>
#include <QDate>
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
        int altRepresentative;
        QString compositeId;
        QDate end;
        QString hourCategory;
        QString openMarking;
        int persNr;
        int representative;
        QDate start;
        QString text;
    };

    const QVector<Absence> &absences() const;

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    QVector<Absence> m_absences;
};
