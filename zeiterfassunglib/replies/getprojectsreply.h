#ifndef GETPROJECTSREPLY_H
#define GETPROJECTSREPLY_H

#include <memory>

#include <QNetworkReply>
#include <QVector>

#include "zeiterfassunglib_global.h"
#include "zeiterfassungreply.h"

class ZeiterfassungApi;

class ZEITERFASSUNGLIBSHARED_EXPORT GetProjectsReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    GetProjectsReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

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

#endif // GETPROJECTSREPLY_H
