#include "advanvedviewdialog.h"
#include "ui_advanvedviewdialog.h"

#include "stripswidget.h"
#include "mainwindow.h"

AdvanvedViewDialog::AdvanvedViewDialog(StripsWidget &stripsWidget) :
    QDialog(&stripsWidget.mainWindow()),
    ui(new Ui::AdvanvedViewDialog),
    m_stripsWidget(stripsWidget)
{
    ui->setupUi(this);
}

AdvanvedViewDialog::~AdvanvedViewDialog()
{
    delete ui;
}
