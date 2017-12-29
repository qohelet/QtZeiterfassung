#pragma once

#include <memory>

#include <QObject>
#include <QWidget>

#include "zeiterfassungguilib_global.h"

class QUiLoader;
class QByteArray;

class ZEITERFASSUNGGUILIBSHARED_EXPORT StripFactory : public QObject
{
    Q_OBJECT

public:
    explicit StripFactory(QObject *parent = Q_NULLPTR);

    bool load(const QString &stripsPath);
    const QString &errorString() const;

    std::unique_ptr<QWidget> createBookingStartStrip(QWidget *parent = Q_NULLPTR);
    std::unique_ptr<QWidget> createBookingEndStrip(QWidget *parent = Q_NULLPTR);
    std::unique_ptr<QWidget> createTimeAssignmentStrip(QWidget *parent = Q_NULLPTR);

private:
    QUiLoader *m_loader;

    QString m_errorString;

    QByteArray m_bookingStartStrip;
    QByteArray m_bookingEndStrip;
    QByteArray m_timeAssignmentStrip;
};
