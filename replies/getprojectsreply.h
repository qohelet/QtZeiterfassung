#ifndef GETPROJECTSREPLY_H
#define GETPROJECTSREPLY_H

#include "zeiterfassungreply.h"
#include "zeiterfassungapi.h"

class QNetworkReply;

class GetProjectsReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    GetProjectsReply(QNetworkReply *reply, ZeiterfassungApi *zeiterfassung);

    const QVector<ZeiterfassungApi::Project> &projects() const;

private Q_SLOTS:
    void requestFinished();

private:
    QNetworkReply *m_reply;
    QVector<ZeiterfassungApi::Project> m_projects;
};

#endif // GETPROJECTSREPLY_H
