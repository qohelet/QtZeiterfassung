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
    Q_INIT_RESOURCE(advancedviewplugin_resources);
}

void AdvancedViewPlugin::attachTo(MainWindow &mainWindow)
{
    for(auto stripsWidget : mainWindow.stripsWidgets())
        stripsWidget->headerLayout()->addWidget(new AdvancedViewWidget(*stripsWidget));
}
