#include "reportsplugin.h"

#include <QDebug>

#include "reportswidget.h"

ReportsPlugin::ReportsPlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{
    qDebug() << "called";
}

void ReportsPlugin::attachTo(MainWindow &mainWindow)
{
    new ReportsWidget(mainWindow);
}
