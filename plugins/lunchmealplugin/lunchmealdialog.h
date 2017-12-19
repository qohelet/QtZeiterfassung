#ifndef LUNCHMEALDIALOG_H
#define LUNCHMEALDIALOG_H

#include <QDialog>

class StripsWidget;

namespace Ui { class LunchMealDialog; }

class LunchMealDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LunchMealDialog(StripsWidget &stripsWidget);
    ~LunchMealDialog();

private:
    Ui::LunchMealDialog *ui;
};

#endif // LUNCHMEALDIALOG_H
