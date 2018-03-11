#include "lunchmealsettingswidget.h"

#include <QFormLayout>
#include <QLineEdit>

LunchMealSettingsWidget::LunchMealSettingsWidget(ZeiterfassungSettings &settings, QWidget *parent) :
    SettingsWidget(parent),
    m_settings(settings)
{
    auto layout = new QFormLayout(this);
    layout->setMargin(0);

    m_lineEditUrl = new QLineEdit(m_settings.url().toString(), this);
    layout->addRow(tr("Lunch meal API:"), m_lineEditUrl);

    m_lineEditDateFormat = new QLineEdit(m_settings.dateFormat(), this);
    layout->addRow(tr("Lunch meal date format:"), m_lineEditDateFormat);

    setLayout(layout);
}

bool LunchMealSettingsWidget::isValid(QString &message) const
{
    Q_UNUSED(message)

    return true;
}

void LunchMealSettingsWidget::apply()
{

}
