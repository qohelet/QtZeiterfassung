#include "lunchmealsettings.h"

#include "zeiterfassungsettings.h"

LunchMealSettings::LunchMealSettings(ZeiterfassungSettings &settings) :
    m_settings(settings)
{

}

QUrl LunchMealSettings::url() const
{
    return m_settings.value(QStringLiteral("LunchMealPlugin/url"),
                            QUrl(QStringLiteral("https://brunner.ninja/lunch/%0.txt")))
            .toUrl();
}

void LunchMealSettings::setUrl(const QUrl &url)
{
    m_settings.setValue(QStringLiteral("LunchMealPlugin/url"), url);
}

QString LunchMealSettings::dateFormat() const
{
    return m_settings.value(QStringLiteral("LunchMealPlugin/dateFormat"), QStringLiteral("yyyy-MM-dd")).toString();
}

void LunchMealSettings::setDateFormat(const QString &dateFormat)
{
    m_settings.setValue(QStringLiteral("LunchMealPlugin/dateFormat"), dateFormat);
}
