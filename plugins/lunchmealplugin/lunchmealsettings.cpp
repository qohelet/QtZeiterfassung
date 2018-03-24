#include "lunchmealsettings.h"

#include "zeiterfassungsettings.h"

const QString LunchMealSettings::m_url("LunchMealPlugin/url");
const QString LunchMealSettings::m_dateFormat("LunchMealPlugin/dateFormat");
const QString LunchMealSettings::m_defaultUrl("https://brunner.ninja/lunch/%0.txt");
const QString LunchMealSettings::m_defaultDateFormat("yyyy-MM-dd");

LunchMealSettings::LunchMealSettings(ZeiterfassungSettings &settings, QObject *parent) :
    QObject(parent),
    m_settings(settings)
{

}

QString LunchMealSettings::url() const
{
    return m_settings.value(m_url, m_defaultUrl).toString();
}

void LunchMealSettings::setUrl(const QString &url)
{
    if(this->url() != url)
    {
        m_settings.setValue(QStringLiteral("LunchMealPlugin/url"), url);
        Q_EMIT urlChanged(url);
    }
}

QString LunchMealSettings::dateFormat() const
{
    return m_settings.value(m_dateFormat, m_defaultDateFormat).toString();
}

void LunchMealSettings::setDateFormat(const QString &dateFormat)
{
    if(this->dateFormat() != dateFormat)
    {
        m_settings.setValue(QStringLiteral("LunchMealPlugin/dateFormat"), dateFormat);
        Q_EMIT dateFormatChanged(dateFormat);
    }
}
