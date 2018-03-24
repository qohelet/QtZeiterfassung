#ifndef UPDATERSETTINGS_H
#define UPDATERSETTINGS_H

#include <QObject>
#include <QUrl>
#include <QDate>

class ZeiterfassungSettings;

class UpdaterSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QDate lastUpdateCheck READ lastUpdateCheck WRITE setLastUpdateCheck NOTIFY lastUpdateCheckChanged)

public:
    explicit UpdaterSettings(ZeiterfassungSettings &settings, QObject *parent = nullptr);

    QUrl url() const;
    void setUrl(const QUrl &url);

    QDate lastUpdateCheck() const;
    void setLastUpdateCheck(const QDate &lastUpdateCheck);

Q_SIGNALS:
    void urlChanged(const QUrl &url);
    void lastUpdateCheckChanged(const QDate &lastUpdateCheck);

private:
    ZeiterfassungSettings &m_settings;

    static const QString m_url;
    static const QString m_lastUpdateCheck;
    static const QUrl m_defaultUrl;
};

#endif // UPDATERSETTINGS_H
