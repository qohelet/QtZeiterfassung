#include "lunchmealsettingswidget.h"

#include <QFormLayout>
#include <QLineEdit>

LunchMealSettingsWidget::LunchMealSettingsWidget(QWidget *parent) :
    SettingsWidget(parent)
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

}

void LunchMealSettingsWidget::apply()
{

}
