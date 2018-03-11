#include "weathersettingswidget.h"

#include <QFormLayout>
#include <QLineEdit>
#include <QDebug>

WeatherSettingsWidget::WeatherSettingsWidget(ZeiterfassungSettings &settings, QWidget *parent) :
    SettingsWidget(parent),
    m_settings(settings)
{
    auto layout = new QFormLayout(this);
    layout->setMargin(0);

    m_lineEdit = new QLineEdit(m_settings.url().toString(), this);
    layout->addRow(tr("Weather API:"), m_lineEdit);

    setLayout(layout);
}

bool WeatherSettingsWidget::isValid(QString &message) const
{
    Q_UNUSED(message)

    qDebug() << "called";
    return true;
}

void WeatherSettingsWidget::apply()
{
    qDebug() << "called";
}
