#ifndef ZEITERFASSUNGPLUGIN_H
#define ZEITERFASSUNGPLUGIN_H

#include <QObject>

#include "zeiterfassunglib_global.h"

class MainWindow;
class StripsWidget;

class ZEITERFASSUNGLIBSHARED_EXPORT ZeiterfassungPlugin : public QObject
{
    Q_OBJECT

public:
    explicit ZeiterfassungPlugin(QObject *parent = Q_NULLPTR);

    virtual void attachTo(MainWindow &mainWindow) { Q_UNUSED(mainWindow) }
};

Q_DECLARE_INTERFACE(ZeiterfassungPlugin, "dbsoftware.zeiterfassung.plugin/1.0")

#endif // ZEITERFASSUNGPLUGIN_H
