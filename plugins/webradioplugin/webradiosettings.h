#ifndef WEBRADIOSETTINGS_H
#define WEBRADIOSETTINGS_H

#include <QUrl>

class ZeiterfassungSettings;

class WebRadioSettings
{
public:
    WebRadioSettings(ZeiterfassungSettings &settings);

    QStringList urls() const;
    void setUrls(const QStringList &urls);

private:
    ZeiterfassungSettings &m_settings;
};

#endif // WEBRADIOSETTINGS_H
