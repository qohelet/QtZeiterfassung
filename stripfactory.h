#ifndef STRIPFACTORY_H
#define STRIPFACTORY_H

#include <QObject>

class QUiLoader;
class QByteArray;

class StripFactory : public QObject
{
    Q_OBJECT

public:
    explicit StripFactory(QObject *parent = Q_NULLPTR);

    bool load(const QString &stripsPath);
    const QString &errorString() const;

    QWidget *createBookingStartStrip(QWidget *parent = Q_NULLPTR);
    QWidget *createBookingEndStrip(QWidget *parent = Q_NULLPTR);
    QWidget *createTimeAssignmentStrip(QWidget *parent = Q_NULLPTR);

private:
    QUiLoader *m_loader;

    QString m_errorString;

    QByteArray m_bookingStartStrip;
    QByteArray m_bookingEndStrip;
    QByteArray m_timeAssignmentStrip;
};

#endif // STRIPFACTORY_H
