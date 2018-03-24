#include "presencesettingswidget.h"

#include <QFormLayout>
#include <QSpinBox>

PresenceSettingsWidget::PresenceSettingsWidget(ZeiterfassungSettings &settings, QWidget *parent) :
    SettingsWidget(parent),
    m_settings(settings)
{
    auto layout = new QFormLayout(this);
    layout->setMargin(0);

    m_spinBox = new QSpinBox(this);
    m_spinBox->setRange(0, std::numeric_limits<int>::max());
    m_spinBox->setValue(m_settings.interval());
    layout->addRow(tr("Interval(ms):"), m_spinBox);

    setLayout(layout);
}

bool PresenceSettingsWidget::isValid(QString &message) const
{
    return true;
}

void PresenceSettingsWidget::apply()
{
    m_settings.setInterval(m_spinBox->value());
}
