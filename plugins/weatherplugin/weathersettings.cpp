#include "weathersettings.h"

#include <QFormLayout>
#include <QLineEdit>
#include <QDebug>

WeatherSettings::WeatherSettings(ZeiterfassungSettings &settings, QWidget *parent) :
    SettingsWidget(parent),
    m_settings(settings)
{
    auto layout = new QFormLayout(this);
    layout->setMargin(0);

    m_lineEdit = new QLineEdit(this);
    layout->addRow(tr("Weather API:"), m_lineEdit);

    setLayout(layout);
}

bool WeatherSettings::isValid(QString &message) const
{
    Q_UNUSED(message)

    qDebug() << "called";
    return true;
}

void WeatherSettings::apply()
{
    qDebug() << "called";
}
