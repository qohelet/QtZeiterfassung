#include "stripfactory.h"

#include <QUiLoader>
#include <QFile>
#include <QDir>
#include <QBuffer>
#include <QDebug>

StripFactory::StripFactory(QObject *parent) :
    QObject(parent),
    m_loader(new QUiLoader(this))
{

}

bool StripFactory::load(const QString &stripsPath)
{
    QDir stripsDir(stripsPath);

    {
        QFile file(stripsDir.absoluteFilePath("bookingstartstrip.ui"));
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            m_errorString = file.errorString();
            return false;
        }
        m_bookingStartStrip = file.readAll();
    }

    {
        QFile file(stripsDir.absoluteFilePath("bookingendstrip.ui"));
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            m_errorString = file.errorString();
            return false;
        }
        m_bookingEndStrip = file.readAll();
    }

    {
        QFile file(stripsDir.absoluteFilePath("timeassignmentstrip.ui"));
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            m_errorString = file.errorString();
            return false;
        }
        m_timeAssignmentStrip = file.readAll();
    }

    return true;
}

const QString &StripFactory::errorString() const
{
    return m_errorString;
}

QWidget *StripFactory::createBookingStartStrip(QWidget *parent)
{
    QBuffer buffer(&m_bookingStartStrip);
    if(!buffer.open(QIODevice::ReadOnly))
    {
        qCritical() << "could not open buffer" << buffer.errorString();
        return Q_NULLPTR;
    }

    return m_loader->load(&buffer, parent);
}

QWidget *StripFactory::createBookingEndStrip(QWidget *parent)
{
    QBuffer buffer(&m_bookingEndStrip);
    if(!buffer.open(QIODevice::ReadOnly))
    {
        qCritical() << "could not open buffer" << buffer.errorString();
        return Q_NULLPTR;
    }

    return m_loader->load(&buffer, parent);
}

QWidget *StripFactory::createTimeAssignmentStrip(QWidget *parent)
{
    QBuffer buffer(&m_timeAssignmentStrip);
    if(!buffer.open(QIODevice::ReadOnly))
    {
        qCritical() << "could not open buffer" << buffer.errorString();
        return Q_NULLPTR;
    }

    return m_loader->load(&buffer, parent);
}