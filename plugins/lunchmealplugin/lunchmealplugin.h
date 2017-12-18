#ifndef LUNCHMEALPLUGIN_H
#define LUNCHMEALPLUGIN_H

#include <QObject>

#include "zeiterfassungplugin.h"

class Q_DECL_EXPORT LunchMealPlugin : public ZeiterfassungPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "dbsoftware.zeiterfassung.plugin/1.0" FILE "lunchmealplugin.json")
    Q_INTERFACES(ZeiterfassungPlugin)

public:
    explicit LunchMealPlugin(QObject *parent = Q_NULLPTR);

    // ZeiterfassungPlugin interface
};

#endif // LUNCHMEALPLUGIN_H
