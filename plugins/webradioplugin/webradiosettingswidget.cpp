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
    for(const auto &url : m_urlsWidget->stringList())
    {
        if(!QUrl::fromUserInput(url).isValid())
        {
            message = tr("A web radio url is invalid!");
            return false;
        }
    }

    return true;
}

void WebRadioSettingsWidget::apply()
{
    if(m_urlsWidget->stringList() != m_settings.urls())
        m_settings.setUrls(m_urlsWidget->stringList());
}
