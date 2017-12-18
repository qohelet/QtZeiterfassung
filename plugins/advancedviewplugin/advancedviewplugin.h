#ifndef ADVANCEDVIEWPLUGIN_H
#define ADVANCEDVIEWPLUGIN_H

#include <QObject>

#include "zeiterfassungplugin.h"

class MainWindow;

class Q_DECL_EXPORT AdvancedViewPlugin : public ZeiterfassungPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "dbsoftware.zeiterfassung.plugin/1.0" FILE "advancedviewplugin.json")
    Q_INTERFACES(ZeiterfassungPlugin)

public:
    explicit AdvancedViewPlugin(QObject *parent = Q_NULLPTR);

    // ZeiterfassungPlugin interface
    void attachTo(MainWindow &mainWindow);
};

#endif // ADVANCEDVIEWPLUGIN_H