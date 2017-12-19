#ifndef PRESENCEPLUGIN_H
#define PRESENCEPLUGIN_H

#include <QObject>
#include <QTranslator>

#include "zeiterfassungplugin.h"

class MainWindow;

class Q_DECL_EXPORT PresencePlugin : public ZeiterfassungPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "dbsoftware.zeiterfassung.plugin/1.0" FILE "presenceplugin.json")
    Q_INTERFACES(ZeiterfassungPlugin)

public:
    explicit PresencePlugin(QObject *parent = Q_NULLPTR);

    // ZeiterfassungPlugin interface
    void attachTo(MainWindow &mainWindow) Q_DECL_OVERRIDE;

private:
    QTranslator m_translator;
};

#endif // PRESENCEPLUGIN_H
