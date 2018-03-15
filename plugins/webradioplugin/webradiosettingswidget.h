#ifndef WEBRADIOSETTINGSWIDGET_H
#define WEBRADIOSETTINGSWIDGET_H

#include <QWidget>

#include "settingswidget.h"

#include "webradiosettings.h"

class QStringListWidget;

class ZeiterfassungSettings;

class WebRadioSettingsWidget : public SettingsWidget
{
    Q_OBJECT

public:
    explicit WebRadioSettingsWidget(ZeiterfassungSettings &settings, QWidget *parent = Q_NULLPTR);

    bool isValid(QString &message) const Q_DECL_OVERRIDE;

public Q_SLOTS:
    void apply() Q_DECL_OVERRIDE;

private:
    WebRadioSettings m_settings;

    QStringListWidget *m_urlsWidget;
};

#endif // WEBRADIOSETTINGSWIDGET_H
