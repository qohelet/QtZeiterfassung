#ifndef EVENTLOOPWITHSTATUS_H
#define EVENTLOOPWITHSTATUS_H

#include <QEventLoop>

class EventLoopWithStatus : public QEventLoop
{
    Q_OBJECT

public:
    EventLoopWithStatus(QObject *parent = Q_NULLPTR);

    bool success() const;
    const QString &message() const;

public Q_SLOTS:
    void quitWithStatus(bool success, const QString &message);

private:
    bool m_success;
    QString m_message;
};

#endif // EVENTLOOPWITHSTATUS_H
