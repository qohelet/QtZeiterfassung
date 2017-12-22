#include "logdialog.h"
#include "ui_logdialog.h"

LogDialog::LogDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogDialog)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
}

LogDialog::~LogDialog()
{
    delete ui;
}

void LogDialog::setModel(QAbstractItemModel *model)
{
    ui->treeView->setModel(model);
}
