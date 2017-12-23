#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include <QDialog>

class QAbstractItemModel;

namespace Ui { class LogDialog; }

class LogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogDialog(QWidget *parent = Q_NULLPTR);
    ~LogDialog();

    void setModel(QAbstractItemModel *model);

private:
    Ui::LogDialog *ui;
};

#endif // LOGDIALOG_H
