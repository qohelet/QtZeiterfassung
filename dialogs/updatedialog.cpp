#include "updatedialog.h"
#include "ui_updatedialog.h"

UpdateDialog::UpdateDialog(ZeiterfassungSettings &settings, QNetworkAccessManager *manager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateDialog),
    m_settings(settings)
{
    ui->setupUi(this);
}

UpdateDialog::~UpdateDialog()
{
    delete ui;
}
