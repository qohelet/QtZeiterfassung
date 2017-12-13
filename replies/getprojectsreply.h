#ifndef GETPROJECTSREPLY_H
#define GETPROJECTSREPLY_H

#include <memory>

#include <QNetworkReply>
#include <QVector>

#include "zeiterfassungreply.h"
#include "zeiterfassungapi.h"

class GetProjectsReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    GetProjectsReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

    const QVector<ZeiterfassungApi::Project> &projects() const;

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    QVector<ZeiterfassungApi::Project> m_projects;
};

#endif // GETPROJECTSREPLY_H
