#ifndef UPDATERPLUGIN_H
#define UPDATERPLUGIN_H

#include <QObject>

#include "zeiterfassungplugin.h"

class MainWindow;

class Q_DECL_EXPORT UpdaterPlugin : public ZeiterfassungPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "dbsoftware.zeiterfassung.plugin/1.0" FILE "updaterplugin.json")
    Q_INTERFACES(ZeiterfassungPlugin)

public:
    explicit UpdaterPlugin(QObject *parent = Q_NULLPTR);

    // ZeiterfassungPlugin interface
    void attachTo(MainWindow &mainWindow);
};

#endif // UPDATERPLUGIN_H
