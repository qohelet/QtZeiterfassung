#pragma once

#include <QDialog>

class QDate;

class ZeiterfassungApi;

namespace Ui { class AbsenceDialog; }

class AbsenceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AbsenceDialog(int userId, const QDate &date, ZeiterfassungApi &erfassung, QWidget *parent = 0);
    ~AbsenceDialog();

private:
    Ui::AbsenceDialog *ui;
};
