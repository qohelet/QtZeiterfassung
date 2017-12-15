#ifndef PRESENCEPLUGIN_H
#define PRESENCEPLUGIN_H

#include <QObject>

#include "zeiterfassungplugin.h"

class Q_DECL_EXPORT PresencePlugin : public ZeiterfassungPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "dbsoftware.zeiterfassung.plugin/1.0" FILE "presenceplugin.json")
    Q_INTERFACES(ZeiterfassungPlugin)

public:
    explicit PresencePlugin(QObject *parent = 0);

    // ZeiterfassungPlugin interface
    void initialize() Q_DECL_OVERRIDE;
};

#endif // PRESENCEPLUGIN_H
