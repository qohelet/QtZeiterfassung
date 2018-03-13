#include "weathersettingswidget.h"

#include <QFormLayout>
#include <QLineEdit>

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
    auto valid = QUrl::fromUserInput(m_lineEdit->text()).isValid();

    if(!valid)
        message = tr("The weather api url is invalid!");

    return valid;
}

void WeatherSettingsWidget::apply()
{
    auto url = QUrl::fromUserInput(m_lineEdit->text());
    if(m_settings.url() != url)
        m_settings.setUrl(url);
}
