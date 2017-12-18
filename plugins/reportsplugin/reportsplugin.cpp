#include "reportsplugin.h"

#include <QDebug>

ReportsPlugin::ReportsPlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{
    qDebug() << "called";
}

void ReportsPlugin::attachTo(MainWindow &mainWindow)
{
}
