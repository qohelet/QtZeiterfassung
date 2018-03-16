#ifndef LUNCHMEALSETTINGS_H
#define LUNCHMEALSETTINGS_H

#include <QUrl>

class ZeiterfassungSettings;

class LunchMealSettings
{
public:
    LunchMealSettings(ZeiterfassungSettings &settings);

    // no QString becuase placeholder %0 encodes wrong in urls!
    QString url() const;
    void setUrl(const QString &url);

    QString dateFormat() const;
    void setDateFormat(const QString &dateFormat);

private:
    ZeiterfassungSettings &m_settings;
};

#endif // LUNCHMEALSETTINGS_H
