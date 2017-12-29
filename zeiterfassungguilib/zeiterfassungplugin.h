#pragma once

#include <QObject>

#include "zeiterfassungguilib_global.h"

class MainWindow;
class StripsWidget;

class ZEITERFASSUNGGUILIBSHARED_EXPORT ZeiterfassungPlugin : public QObject
{
    Q_OBJECT

public:
    explicit ZeiterfassungPlugin(QObject *parent = Q_NULLPTR);

    virtual void attachTo(MainWindow &mainWindow) { Q_UNUSED(mainWindow) }
};

Q_DECLARE_INTERFACE(ZeiterfassungPlugin, "dbsoftware.zeiterfassung.plugin/1.0")
