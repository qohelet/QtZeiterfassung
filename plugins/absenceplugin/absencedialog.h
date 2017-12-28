#pragma once

#include <QDialog>

class QDate;

namespace Ui { class AbsenceDialog; }

class AbsenceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AbsenceDialog(const QDate &date, QWidget *parent = 0);
    ~AbsenceDialog();

private:
    Ui::AbsenceDialog *ui;
};
