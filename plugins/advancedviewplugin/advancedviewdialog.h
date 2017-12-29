#pragma once

#include "zeiterfassungdialog.h"

namespace Ui { class AdvancedViewDialog; }
class StripsWidget;
class BookingsModel;
class TimeAssignmentsModel;

class AdvancedViewDialog : public ZeiterfassungDialog
{
    Q_OBJECT

public:
    explicit AdvancedViewDialog(StripsWidget &stripsWidget, QWidget *parent = Q_NULLPTR);
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
