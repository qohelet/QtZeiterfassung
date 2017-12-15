#include "presenceplugin.h"

#include <QDebug>

PresencePlugin::PresencePlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{

}

void PresencePlugin::initialize()
{
    qDebug() << "called";
}
