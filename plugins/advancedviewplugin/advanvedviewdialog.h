#ifndef ADVANVEDVIEWDIALOG_H
#define ADVANVEDVIEWDIALOG_H

#include <QDialog>

namespace Ui { class AdvanvedViewDialog; }
class StripsWidget;
class BookingsModel;
class TimeAssignmentsModel;

class AdvanvedViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdvanvedViewDialog(StripsWidget &stripsWidget);
    ~AdvanvedViewDialog();

private Q_SLOTS:
    void contextMenuBooking(const QPoint &pos);
    void contextMenuTimeAssignment(const QPoint &pos);

private:
    Ui::AdvanvedViewDialog *ui;

    StripsWidget &m_stripsWidget;

    BookingsModel *m_bookingsModel;
    TimeAssignmentsModel *m_timeAssignmentsModel;
};

#endif // ADVANVEDVIEWDIALOG_H
