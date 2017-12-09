#ifndef STRIPSWIDGET_H
#define STRIPSWIDGET_H

#include <QWidget>
#include <QTime>

#include "zeiterfassung.h"

class QBoxLayout;
template <class Key, class T> class QMap;
template <typename T> class QVector;

class ZeiterfassungSettings;

class StripsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StripsWidget(const ZeiterfassungSettings &settings, const QMap<QString, QString> &projects,
                          QWidget *parent = 0);

    bool createStrips(const QVector<Zeiterfassung::Booking> &bookings,
                      const QVector<Zeiterfassung::TimeAssignment> &timeAssignments);
    void clearStrips();

    const QTime timeAssignmentTime() const;
    const QTime lastTimeAssignmentStart() const;

private:
    QString buildProjectString(const QString &project);

    const ZeiterfassungSettings &m_settings;
    const QMap<QString, QString> &m_projects;

    QBoxLayout *m_layout;

    QTime m_timeAssignmentTime;
    QTime m_lastTimeAssignmentStart;
};

#endif // STRIPSWIDGET_H
