#include "advancedviewplugin.h"

#include <QBoxLayout>

#include "mainwindow.h"
#include "stripswidget.h"
#include "advancedviewwidget.h"

AdvancedViewPlugin::AdvancedViewPlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{
}

void AdvancedViewPlugin::attachTo(MainWindow &mainWindow)
{
    for(auto irgendwas : mainWindow.stripsWidgets())
        irgendwas->headerLayout()->addWidget(new AdvancedViewWidget(mainWindow));
}
