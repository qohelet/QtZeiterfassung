#ifndef PRESENCEPLUGIN_H
#define PRESENCEPLUGIN_H

#include <QObject>

class Q_DECL_EXPORT PresencePlugin : public QObject
{
    Q_OBJECT
public:
    explicit PresencePlugin(QObject *parent = 0);

signals:

public slots:
};

#endif // PRESENCEPLUGIN_H
