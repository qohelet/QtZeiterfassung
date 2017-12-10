#include "zeiterfassungreply.h"

#include "zeiterfassungapi.h"

ZeiterfassungReply::ZeiterfassungReply(ZeiterfassungApi *zeiterfassung) :
    QObject(zeiterfassung),
    m_zeiterfassung(zeiterfassung)
{

}

ZeiterfassungApi *ZeiterfassungReply::zeiterfassung() const
{
    return m_zeiterfassung;
}
