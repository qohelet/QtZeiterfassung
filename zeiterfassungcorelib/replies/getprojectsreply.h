#pragma once

#include <memory>

#include <QNetworkReply>
#include <QVector>

#include "zeiterfassungcorelib_global.h"
#include "zeiterfassungreply.h"

class ZeiterfassungApi;

class ZEITERFASSUNGCORELIBSHARED_EXPORT GetProjectsReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    explicit GetProjectsReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

    struct Project
    {
        QString label;
        QString value;
    };

    const QVector<Project> &projects() const;

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    QVector<Project> m_projects;
};
