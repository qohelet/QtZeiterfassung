#ifndef WEATHERPLUGIN_H
#define WEATHERPLUGIN_H

#include <QObject>

class Q_DECL_EXPORT WeatherPlugin : public QObject
{
    Q_OBJECT
public:
    explicit WeatherPlugin(QObject *parent = 0);

signals:

public slots:
};

#endif // WEATHERPLUGIN_H
