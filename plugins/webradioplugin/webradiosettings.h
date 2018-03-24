#ifndef WEBRADIOSETTINGS_H
#define WEBRADIOSETTINGS_H

#include <QObject>
#include <QUrl>

class ZeiterfassungSettings;

class WebRadioSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList urls READ urls WRITE setUrls NOTIFY urlsChanged)
    Q_PROPERTY(QString lastUrl READ lastUrl WRITE setLastUrl NOTIFY lastUrlChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)

public:
    WebRadioSettings(ZeiterfassungSettings &settings, QObject *parent = Q_NULLPTR);

    QStringList urls() const;
    void setUrls(const QStringList &urls);

    QString lastUrl() const;
    void setLastUrl(const QString &lastUrl);

    int volume() const;
    void setVolume(int volume);

Q_SIGNALS:
    void urlsChanged(const QStringList &urls);
    void lastUrlChanged(const QString &lastUrl);
    void volumeChanged(int volume);

private:
    ZeiterfassungSettings &m_settings;

    static const QString m_urls;
    static const QString m_lastUrl;
    static const QString m_volume;
    static const QStringList m_defaultUrls;
    static const int m_defaultVolume;
};

#endif // WEBRADIOSETTINGS_H
