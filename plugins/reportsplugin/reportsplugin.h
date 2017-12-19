#ifndef REPORTSPLUGIN_H
#define REPORTSPLUGIN_H

#include <QObject>
#include <QTranslator>

#include "zeiterfassungplugin.h"

class MainWindow;

class Q_DECL_EXPORT ReportsPlugin : public ZeiterfassungPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "dbsoftware.zeiterfassung.plugin/1.0" FILE "reportsplugin.json")
    Q_INTERFACES(ZeiterfassungPlugin)

public:
    explicit ReportsPlugin(QObject *parent = Q_NULLPTR);

    // ZeiterfassungPlugin interface
    void attachTo(MainWindow &mainWindow) Q_DECL_OVERRIDE;

private:
    QTranslator m_translator;
};

#endif // REPORTSPLUGIN_H
