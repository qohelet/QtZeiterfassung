#include "lunchmealplugin.h"

#include <QDebug>

LunchMealPlugin::LunchMealPlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{

}

void LunchMealPlugin::initialize()
{
    qDebug() << "called";
}
