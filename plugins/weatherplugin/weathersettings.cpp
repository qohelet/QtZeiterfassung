#include "weathersettings.h"

#include "zeiterfassungsettings.h"

WeatherSettings::WeatherSettings(ZeiterfassungSettings &settings) :
    m_settings(settings)
{

}

QUrl WeatherSettings::url() const
{
    return m_settings.value(QStringLiteral("WeatherPlugin/url"),
                            QUrl(QStringLiteral("http://api.openweathermap.org/data/2.5/weather?q=Graz,AT&units=metric&APPID=40f6c892c6162680c6c9235169dc9f83")))
            .toUrl();
}

void WeatherSettings::setUrl(const QUrl &url)
{
    m_settings.setValue(QStringLiteral("WeatherPlugin/url"), url);
}
