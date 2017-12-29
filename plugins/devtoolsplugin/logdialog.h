#pragma once

#include "zeiterfassungdialog.h"

class QAbstractItemModel;

namespace Ui { class LogDialog; }

class LogDialog : public ZeiterfassungDialog
{
    Q_OBJECT

public:
    explicit LogDialog(QWidget *parent = Q_NULLPTR);
    ~LogDialog();

    void setModel(QAbstractItemModel *model);

private:
    Ui::LogDialog *ui;
};
