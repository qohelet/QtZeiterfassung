#include "updaterplugin.h"

#include <QDebug>

#include "mainwindow.h"
#include "zeiterfassungsettings.h"
#include "zeiterfassungapi.h"

#include "updaterdialog.h"

UpdaterPlugin::UpdaterPlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{
    qDebug() << "called";
}

void UpdaterPlugin::attachTo(MainWindow &mainWindow)
{
    qDebug() << "called";

    if(mainWindow.settings().value(QStringLiteral("UpdaterPlugin/lastUpdateCheck")).toDate().isNull() ||
       mainWindow.settings().value(QStringLiteral("UpdaterPlugin/lastUpdateCheck")).toDate() < QDate::currentDate())
        new UpdaterDialog(mainWindow);
}
