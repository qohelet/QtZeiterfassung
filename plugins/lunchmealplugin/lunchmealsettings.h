#ifndef LUNCHMEALSETTINGS_H
#define LUNCHMEALSETTINGS_H

#include <QUrl>

class ZeiterfassungSettings;

class LunchMealSettings
{
public:
    LunchMealSettings(ZeiterfassungSettings &settings);

    QUrl url() const;
    void setUrl(const QUrl &url);

    QString dateFormat() const;
    void setDateFormat(const QString &dateFormat);

private:
    ZeiterfassungSettings &m_settings;
};

#endif // LUNCHMEALSETTINGS_H
