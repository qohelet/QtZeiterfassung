#pragma once

#include <QObject>
#include <QTranslator>

#include "zeiterfassungplugin.h"

class Q_DECL_EXPORT WebRadioPlugin : public ZeiterfassungPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "dbsoftware.zeiterfassung.plugin/1.0" FILE "webradioplugin.json")
    Q_INTERFACES(ZeiterfassungPlugin)

public:
    explicit WebRadioPlugin(QObject *parent = Q_NULLPTR);

    // ZeiterfassungPlugin interface
    void attachTo(MainWindow &mainWindow) Q_DECL_OVERRIDE;

    SettingsWidget *settingsWidget(ZeiterfassungSettings &settings, QWidget *parent = Q_NULLPTR) const Q_DECL_OVERRIDE;

private:
    QTranslator m_translator;
};
