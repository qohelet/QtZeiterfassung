#include "getabsencesreply.h"

GetAbsencesReply::GetAbsencesReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung) :
    ZeiterfassungReply(zeiterfassung),
    m_reply(std::move(reply))
{
    connect(m_reply.get(), &QNetworkReply::finished, this, &GetAbsencesReply::requestFinished);
}

const QVector<GetAbsencesReply::Absence> &GetAbsencesReply::getAbsences() const
{
    return m_absences;
}

void GetAbsencesReply::requestFinished()
{
    //TODO
}
