#include "webradiosettingswidget.h"

#include <QFormLayout>

#include "QStringListWidget/qstringlistwidget.h"

WebRadioSettingsWidget::WebRadioSettingsWidget(ZeiterfassungSettings &settings, QWidget *parent) :
    SettingsWidget(parent),
    m_settings(settings)
{
    auto layout = new QFormLayout(this);
    layout->setMargin(0);

    m_urlsWidget = new QStringListWidget(m_settings.urls(), this);
    layout->addRow(tr("URLs:"), m_urlsWidget);

    setLayout(layout);
}

bool WebRadioSettingsWidget::isValid(QString &message) const
{

}

void WebRadioSettingsWidget::apply()
{

}
