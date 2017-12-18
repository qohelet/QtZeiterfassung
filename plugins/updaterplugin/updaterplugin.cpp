#include "updaterplugin.h"

#include <QDebug>

UpdaterPlugin::UpdaterPlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{
    qDebug() << "called";
}

void UpdaterPlugin::attachTo(MainWindow &mainWindow)
{

}
