#include "weatherwidget.h"

#include "mainwindow.h"

WeatherWidget::WeatherWidget(MainWindow &mainWindow) :
    QLabel(&mainWindow),
    m_mainWindow(mainWindow)
{
    setText(tr("Weather"));
}
