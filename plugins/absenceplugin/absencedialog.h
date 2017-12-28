#pragma once

#include <QDialog>

namespace Ui { class AbsenceDialog; }

class AbsenceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AbsenceDialog(QWidget *parent = 0);
    ~AbsenceDialog();

private:
    Ui::AbsenceDialog *ui;
};
