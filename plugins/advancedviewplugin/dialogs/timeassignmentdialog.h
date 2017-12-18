#ifndef TIMEASSIGNMENTDIALOG_H
#define TIMEASSIGNMENTDIALOG_H

#include <QDialog>
#include <QTime>

#include "zeiterfassunglib_global.h"

template <class Key, class T> class QMap;

class ZeiterfassungSettings;

namespace Ui { class TimeAssignmentDialog; }

class ZEITERFASSUNGLIBSHARED_EXPORT TimeAssignmentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TimeAssignmentDialog(const QMap<QString, QString> &projects, const ZeiterfassungSettings &settings,
                                  QWidget *parent = Q_NULLPTR);
    ~TimeAssignmentDialog();

    QTime getTime() const;
    void setTime(const QTime &time);

    QTime getTimespan() const;
    void setTimespan(const QTime &timespan);

    QString getProject() const;
    void setProject(const QString &project);

    QString getSubproject() const;
    void setSubproject(const QString &subproject);

    QString getWorkpackage() const;
    void setWorkpackage(const QString &workpackage);

    QString getText() const;
    void setText(const QString &text);

private:
    Ui::TimeAssignmentDialog *ui;
};

#endif // TIMEASSIGNMENTDIALOG_H
