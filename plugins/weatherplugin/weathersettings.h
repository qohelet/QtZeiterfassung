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

Q_SIGNALS:
    void urlChanged(const QUrl &url);

private:
    ZeiterfassungSettings &m_settings;

    static const QString m_url;
    static const QUrl m_defaultUrl;
};

#endif // WEATHERSETTINGS_H
