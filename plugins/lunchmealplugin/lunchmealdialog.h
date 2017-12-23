#pragma once

#include <QDialog>

class QDate;

class StripsWidget;

namespace Ui { class LunchMealDialog; }

class LunchMealDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LunchMealDialog(const QDate &date, const QString &content, QWidget *parent = Q_NULLPTR);
    ~LunchMealDialog();

private:
    Ui::LunchMealDialog *ui;
};
