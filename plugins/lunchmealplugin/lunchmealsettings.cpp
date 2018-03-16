#include "lunchmealsettings.h"

#include "zeiterfassungsettings.h"

LunchMealSettings::LunchMealSettings(ZeiterfassungSettings &settings) :
    m_settings(settings)
{

}

QString LunchMealSettings::url() const
{
    return m_settings.value(QStringLiteral("LunchMealPlugin/url"),
                            QStringLiteral("https://brunner.ninja/lunch/%0.txt"))
            .toString();
}

void LunchMealSettings::setUrl(const QString &url)
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
