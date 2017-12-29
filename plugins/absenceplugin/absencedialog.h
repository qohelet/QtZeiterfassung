#pragma once

#include "zeiterfassungdialog.h"

class QDate;

class ZeiterfassungApi;

class AbsencesModel;
namespace Ui { class AbsenceDialog; }

class AbsenceDialog : public ZeiterfassungDialog
{
    Q_OBJECT

public:
    explicit AbsenceDialog(int userId, const QDate &date, ZeiterfassungApi &erfassung, QWidget *parent = 0);
    ~AbsenceDialog();

private Q_SLOTS:
    void errorOccured(const QString &message);

private:
    Ui::AbsenceDialog *ui;

    AbsencesModel *m_model;
};
