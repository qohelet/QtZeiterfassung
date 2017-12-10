#ifndef STRIPSWIDGET_H
#define STRIPSWIDGET_H

#include <QWidget>
#include <QTime>

#include "zeiterfassungapi.h"

class QBoxLayout;
template <class Key, class T> class QMap;
template <typename T> class QVector;

class StripFactory;

class StripsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StripsWidget(StripFactory *stripFactory, const QMap<QString, QString> &projects,
                          QWidget *parent = Q_NULLPTR);

    bool createStrips(const QVector<ZeiterfassungApi::Booking> &bookings,
                      const QVector<ZeiterfassungApi::TimeAssignment> &timeAssignments);
    void clearStrips();

    const QTime timeAssignmentTime() const;
    const QTime lastTimeAssignmentStart() const;

private:
    QString buildProjectString(const QString &project);

    QWidget *appendBookingStartStrip(int id, const QTime &time);
    QWidget *appendBookingEndStrip(int id, const QTime &time);
    QWidget *appendTimeAssignmentStrip(int id, const QTime &duration, const QString &project, const QString &subproject,
                                       const QString &workpackage, const QString &text);

    StripFactory *m_stripFactory;
    const QMap<QString, QString> &m_projects;

    QBoxLayout *m_layout;

    QTime m_timeAssignmentTime;
    QTime m_lastTimeAssignmentStart;
};

#endif // STRIPSWIDGET_H
