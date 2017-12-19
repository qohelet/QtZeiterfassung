#ifndef WEATHERWIDGET_H
#define WEATHERWIDGET_H

#include <memory>

#include <QLabel>
#include <QNetworkReply>

class MainWindow;

class WeatherWidget : public QLabel
{
    Q_OBJECT

public:
    explicit WeatherWidget(MainWindow &mainWindow);

private Q_SLOTS:
    void refresh();
    void finished();

private:
    MainWindow &m_mainWindow;

    std::unique_ptr<QNetworkReply> m_reply;
};

#endif // WEATHERWIDGET_H
