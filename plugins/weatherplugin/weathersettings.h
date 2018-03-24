#ifndef WEATHERSETTINGS_H
#define WEATHERSETTINGS_H

#include <QObject>
#include <QUrl>

class ZeiterfassungSettings;

class WeatherSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)

public:
    WeatherSettings(ZeiterfassungSettings &settings, QObject *parent = Q_NULLPTR);

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
