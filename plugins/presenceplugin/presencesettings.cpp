#include "presencesettings.h"

#include "zeiterfassungsettings.h"

const QString PresenceSettings::m_interval("PresencePlugin/interval");
const int PresenceSettings::m_defaultInterval(60000);

PresenceSettings::PresenceSettings(ZeiterfassungSettings &settings, QObject *parent) :
    QObject(parent),
    m_settings(settings)
{

}

int PresenceSettings::interval() const
{
    return m_settings.value(m_interval, m_defaultInterval).toInt();
}

void PresenceSettings::setInterval(int interval)
{
    if(this->interval() != interval)
    {
        m_settings.setValue(m_interval, interval);
        Q_EMIT intervalChanged(interval);
    }
}
