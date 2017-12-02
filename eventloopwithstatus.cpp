#include "eventloopwithstatus.h"

EventLoopWithStatus::EventLoopWithStatus(QObject *parent) :
    QEventLoop(parent)
{

}

bool EventLoopWithStatus::success() const
{
    return m_success;
}

const QString &EventLoopWithStatus::message() const
{
    return m_message;
}

void EventLoopWithStatus::quitWithStatus(bool success, const QString &message)
{
    m_success = success;
    m_message = message;
    quit();
}
