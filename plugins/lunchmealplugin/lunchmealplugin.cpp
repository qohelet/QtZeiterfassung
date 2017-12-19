#include "lunchmealplugin.h"

#include <QDebug>
#include <QBoxLayout>

#include "mainwindow.h"
#include "stripswidget.h"

#include "lunchmealwidget.h"

LunchMealPlugin::LunchMealPlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{
    qDebug() << "called";
}

void LunchMealPlugin::attachTo(MainWindow &mainWindow)
{
    for(auto stripsWidget : mainWindow.stripsWidgets())
        stripsWidget->headerLayout()->addWidget(new LunchMealWidget(*stripsWidget));
}
