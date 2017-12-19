#ifndef WEATHERPLUGIN_H
#define WEATHERPLUGIN_H

#include <QObject>

#include "zeiterfassungplugin.h"

class Q_DECL_EXPORT WeatherPlugin : public ZeiterfassungPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "dbsoftware.zeiterfassung.plugin/1.0" FILE "weatherplugin.json")
    Q_INTERFACES(ZeiterfassungPlugin)

public:
    explicit WeatherPlugin(QObject *parent = Q_NULLPTR);

    // ZeiterfassungPlugin interface
    void attachTo(MainWindow &mainWindow) Q_DECL_OVERRIDE;
};

#endif // WEATHERPLUGIN_H
