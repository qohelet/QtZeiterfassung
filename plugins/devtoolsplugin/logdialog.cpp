#include "logdialog.h"
#include "ui_logdialog.h"

LogDialog::LogDialog(QWidget *parent) :
    ZeiterfassungDialog(parent),
    ui(new Ui::LogDialog)
{
    ui->setupUi(this);
}

LogDialog::~LogDialog()
{
    delete ui;
}

void LogDialog::setModel(QAbstractItemModel *model)
{
    ui->treeView->setModel(model);
}
