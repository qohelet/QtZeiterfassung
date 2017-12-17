#include "zeiterfassungreply.h"

#include "zeiterfassungapi.h"

ZeiterfassungReply::ZeiterfassungReply(ZeiterfassungApi *zeiterfassung) :
    QObject(zeiterfassung),
    m_zeiterfassung(zeiterfassung),
    m_success(false)
{

}

bool ZeiterfassungReply::success() const
{
    return m_success;
}

const QString &ZeiterfassungReply::message() const
{
    return m_message;
}

ZeiterfassungApi *ZeiterfassungReply::zeiterfassung() const
{
    return m_zeiterfassung;
}

void ZeiterfassungReply::setSuccess(bool success)
{
    m_success = success;
}

void ZeiterfassungReply::setMessage(const QString &message)
{
    m_message = message;
}
