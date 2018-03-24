#include "weathersettings.h"

#include "zeiterfassungsettings.h"

const QString WeatherSettings::m_url("WeatherPlugin/url");
const QUrl WeatherSettings::m_defaultUrl(QStringLiteral("http://api.openweathermap.org/data/2.5/weather?q=Graz,AT&units=metric&APPID=40f6c892c6162680c6c9235169dc9f83"));

WeatherSettings::WeatherSettings(ZeiterfassungSettings &settings) :
    m_settings(settings)
{

}

QUrl WeatherSettings::url() const
{
    return m_settings.value(m_url, m_defaultUrl).toUrl();
}

void WeatherSettings::setUrl(const QUrl &url)
{
    if(this->url() != url)
    {
        m_settings.setValue(m_url, url);
        Q_EMIT urlChanged(url);
    }
}
