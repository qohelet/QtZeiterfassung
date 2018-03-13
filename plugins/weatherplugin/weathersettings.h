#ifndef WEATHERSETTINGS_H
#define WEATHERSETTINGS_H

#include <QUrl>

class ZeiterfassungSettings;

class WeatherSettings
{
public:
    WeatherSettings(ZeiterfassungSettings &settings);

    QUrl url() const;
    void setUrl(const QUrl &url);

private:
    ZeiterfassungSettings &m_settings;
};

#endif // WEATHERSETTINGS_H
