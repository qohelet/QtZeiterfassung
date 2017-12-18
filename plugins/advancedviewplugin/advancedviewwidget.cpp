#include "advancedviewwidget.h"

#include "mainwindow.h"

AdvancedViewWidget::AdvancedViewWidget(MainWindow &mainWindow) :
    QToolButton(&mainWindow),
    m_mainWindow(mainWindow)
{
    setText(tr("X"));
}
