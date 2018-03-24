#include "updatersettings.h"

#include "zeiterfassungsettings.h"

const QString UpdaterSettings::m_url("UpdaterPlugin/url");
const QString UpdaterSettings::m_lastUpdateCheck("UpdaterPlugin/lastUpdateCheck");
const QUrl UpdaterSettings::m_defaultUrl(QStringLiteral("https://api.github.com/repos/0xFEEDC0DE64/QtZeiterfassung/releases"));

UpdaterSettings::UpdaterSettings(ZeiterfassungSettings &settings, QObject *parent) :
    QObject(parent),
    m_settings(settings)
{

}

QUrl UpdaterSettings::url() const
{
    return m_settings.value(m_url, m_defaultUrl).toUrl();
}

void UpdaterSettings::setUrl(const QUrl &url)
{
    if(this->url() != url)
    {
        m_settings.setValue(m_url, url);
        Q_EMIT urlChanged(url);
    }
}

QDate UpdaterSettings::lastUpdateCheck() const
{
    return m_settings.value(m_lastUpdateCheck).toDate();
}

void UpdaterSettings::setLastUpdateCheck(const QDate &lastUpdateCheck)
{
    if(this->lastUpdateCheck() != lastUpdateCheck)
    {
        m_settings.setValue(m_lastUpdateCheck, lastUpdateCheck);
        Q_EMIT lastUpdateCheckChanged(lastUpdateCheck);
    }
}
