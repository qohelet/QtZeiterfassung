#pragma once

#include <QObject>
#include <QTranslator>

#include "zeiterfassungplugin.h"

class MainWindow;

class Q_DECL_EXPORT LunchMealPlugin : public ZeiterfassungPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "dbsoftware.zeiterfassung.plugin/1.0" FILE "lunchmealplugin.json")
    Q_INTERFACES(ZeiterfassungPlugin)

public:
    explicit LunchMealPlugin(QObject *parent = Q_NULLPTR);

    // ZeiterfassungPlugin interface
    void attachTo(MainWindow &mainWindow) Q_DECL_OVERRIDE;

    virtual SettingsWidget *settingsWidget(ZeiterfassungSettings &settings, QWidget *parent = Q_NULLPTR) const Q_DECL_OVERRIDE;

private:
    QTranslator m_translator;
};
