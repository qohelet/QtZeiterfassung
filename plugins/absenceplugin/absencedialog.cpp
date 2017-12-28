#include "absencedialog.h"
#include "ui_absencedialog.h"

AbsenceDialog::AbsenceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AbsenceDialog)
{
    ui->setupUi(this);
}

AbsenceDialog::~AbsenceDialog()
{
    delete ui;
}
