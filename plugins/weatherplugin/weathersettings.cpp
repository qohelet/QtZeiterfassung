#include "weathersettings.h"

#include <QFormLayout>
#include <QLineEdit>
#include <QDebug>

WeatherSettings::WeatherSettings(QWidget *parent) :
    SettingsWidget(parent)
{
    auto layout = new QFormLayout(this);
    layout->setMargin(0);

    layout->addRow(tr("Weather API:"), new QLineEdit(this));

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
