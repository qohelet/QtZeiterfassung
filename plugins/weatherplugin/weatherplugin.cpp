#include "weatherplugin.h"

#include <QDebug>
#include <QStatusBar>

#include "mainwindow.h"

#include "weatherwidget.h"

WeatherPlugin::WeatherPlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{
    qDebug() << "called";
}

void WeatherPlugin::attachTo(MainWindow &mainWindow)
{
    mainWindow.statusBar()->addWidget(new WeatherWidget(mainWindow));
}
