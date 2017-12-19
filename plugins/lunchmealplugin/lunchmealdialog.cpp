#include "lunchmealdialog.h"
#include "ui_lunchmealdialog.h"

#include "stripswidget.h"
#include "mainwindow.h"

LunchMealDialog::LunchMealDialog(StripsWidget &stripsWidget) :
    QDialog(&stripsWidget.mainWindow()),
    ui(new Ui::LunchMealDialog)
{
    ui->setupUi(this);
}

LunchMealDialog::~LunchMealDialog()
{
    delete ui;
}
