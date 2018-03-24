#include "lunchmealsettingswidget.h"

#include <QFormLayout>
#include <QLineEdit>

LunchMealSettingsWidget::LunchMealSettingsWidget(ZeiterfassungSettings &settings, QWidget *parent) :
    SettingsWidget(parent),
    m_settings(settings)
{
    auto layout = new QFormLayout(this);
    layout->setMargin(0);

    m_lineEditUrl = new QLineEdit(m_settings.url(), this);
    layout->addRow(tr("Lunch meal API:"), m_lineEditUrl);

    m_lineEditDateFormat = new QLineEdit(m_settings.dateFormat(), this);
    layout->addRow(tr("Lunch meal date format:"), m_lineEditDateFormat);

    setLayout(layout);
}

bool LunchMealSettingsWidget::isValid(QString &message) const
{
    auto valid = QUrl::fromUserInput(m_lineEditUrl->text()).isValid();

    if(!valid)
        message = tr("The lunch meal api url is invalid!");

    return valid;
}

void LunchMealSettingsWidget::apply()
{
    m_settings.setUrl(m_lineEditUrl->text());
    m_settings.setDateFormat(m_lineEditDateFormat->text());
}
