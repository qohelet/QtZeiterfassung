#ifndef ADVANCEDVIEWDIALOG_H
#define ADVANCEDVIEWDIALOG_H

#include <QDialog>

namespace Ui { class AdvancedViewDialog; }
class StripsWidget;
class BookingsModel;
class TimeAssignmentsModel;

class AdvancedViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdvancedViewDialog(StripsWidget &stripsWidget);
    ~AdvancedViewDialog();

private Q_SLOTS:
    void contextMenuBooking(const QPoint &pos);
    void contextMenuTimeAssignment(const QPoint &pos);

private:
    Ui::AdvancedViewDialog *ui;

    StripsWidget &m_stripsWidget;

    BookingsModel *m_bookingsModel;
    TimeAssignmentsModel *m_timeAssignmentsModel;
};

#endif // ADVANCEDVIEWDIALOG_H
