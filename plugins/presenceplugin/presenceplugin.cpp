#include "presenceplugin.h"

#include <QDebug>

#include "mainwindow.h"
#include "presencewidget.h"

PresencePlugin::PresencePlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{
    qDebug() << "called";
}

void PresencePlugin::attachTo(MainWindow &mainWindow)
{
    new PresenceWidget(mainWindow);
}
