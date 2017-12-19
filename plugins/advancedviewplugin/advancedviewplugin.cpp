#include "advancedviewplugin.h"

#include <QDebug>
#include <QBoxLayout>

#include "mainwindow.h"
#include "stripswidget.h"

#include "advancedviewwidget.h"

AdvancedViewPlugin::AdvancedViewPlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{
    qDebug() << "called";
}

void AdvancedViewPlugin::attachTo(MainWindow &mainWindow)
{
    for(auto stripsWidget : mainWindow.stripsWidgets())
        stripsWidget->headerLayout()->addWidget(new AdvancedViewWidget(*stripsWidget));
}
