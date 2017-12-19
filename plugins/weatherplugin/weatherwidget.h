#ifndef WEATHERWIDGET_H
#define WEATHERWIDGET_H

#include <QLabel>

class MainWindow;

class WeatherWidget : public QLabel
{
    Q_OBJECT

public:
    explicit WeatherWidget(MainWindow &mainWindow);

private:
    MainWindow &m_mainWindow;
};

#endif // WEATHERWIDGET_H
