#include "updatersettingswidget.h"

#include <QFormLayout>
#include <QLineEdit>

UpdaterSettingsWidget::UpdaterSettingsWidget(ZeiterfassungSettings &settings, QWidget *parent) :
    SettingsWidget(parent),
    m_settings(settings)
{
    auto layout = new QFormLayout(this);
    layout->setMargin(0);

    m_lineEdit = new QLineEdit(m_settings.url().toString(), this);
    layout->addRow(tr("Updater url:"), m_lineEdit);

    setLayout(layout);
}

bool UpdaterSettingsWidget::isValid(QString &message) const
{
    auto valid = QUrl::fromUserInput(m_lineEdit->text()).isValid();

    if(!valid)
        message = tr("The updater url is invalid!");

    return valid;
}

void UpdaterSettingsWidget::apply()
{
    m_settings.setUrl(QUrl(m_lineEdit->text()));
}
