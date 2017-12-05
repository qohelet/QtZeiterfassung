#ifndef TIMEASSIGNMENTSTRIP_H
#define TIMEASSIGNMENTSTRIP_H

#include <QFrame>

class QTime;

class ZeiterfassungSettings;
namespace Ui { class TimeAssignmentStrip; }

class TimeAssignmentStrip : public QFrame
{
    Q_OBJECT

public:
    explicit TimeAssignmentStrip(int id, const QTime &duration, const QString &projekt, const QString &subprojekt,
                                 const QString &workpackage, const QString &text, const ZeiterfassungSettings &settings,
                                 QWidget *parent = 0);
    ~TimeAssignmentStrip();

private:
    Ui::TimeAssignmentStrip *ui;
};

#endif // TIMEASSIGNMENTSTRIP_H
