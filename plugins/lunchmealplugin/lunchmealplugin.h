#ifndef LUNCHMEALPLUGIN_H
#define LUNCHMEALPLUGIN_H

#include <QObject>

class Q_DECL_EXPORT LunchMealPlugin : public QObject
{
    Q_OBJECT
public:
    explicit LunchMealPlugin(QObject *parent = 0);

signals:

public slots:
};

#endif // LUNCHMEALPLUGIN_H
