#include "weatherplugin.h"

#include <QDebug>

WeatherPlugin::WeatherPlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{

}

void WeatherPlugin::initialize()
{
    qDebug() << "called";
}
