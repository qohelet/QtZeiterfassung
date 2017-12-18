#include "lunchmealplugin.h"

#include <QDebug>

LunchMealPlugin::LunchMealPlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{
    qDebug() << "called";
}
