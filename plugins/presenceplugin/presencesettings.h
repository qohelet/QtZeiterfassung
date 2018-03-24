#ifndef PRESENCESETTINGS_H
#define PRESENCESETTINGS_H

#include <QObject>

class ZeiterfassungSettings;

class PresenceSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int interval READ interval WRITE setInterval NOTIFY intervalChanged)

public:
    PresenceSettings(ZeiterfassungSettings &settings, QObject *parent = Q_NULLPTR);

    int interval() const;
    void setInterval(int interval);

Q_SIGNALS:
    void intervalChanged(int interval);

private:
    ZeiterfassungSettings &m_settings;

    static const QString m_interval;
    static const int m_defaultInterval;
};

#endif // PRESENCESETTINGS_H
