#include "authenticationdialog.h"
#include "ui_authenticationdialog.h"

AuthenticationDialog::AuthenticationDialog(QWidget *parent) :
    ZeiterfassungDialog(parent),
    ui(new Ui::AuthenticationDialog)
{
    ui->setupUi(this);
}

AuthenticationDialog::~AuthenticationDialog()
{
    delete ui;
}

QString AuthenticationDialog::username() const
{
    return ui->lineEditUsername->text();
}

void AuthenticationDialog::setUsername(const QString &username)
{
    ui->lineEditUsername->setText(username);
}

QString AuthenticationDialog::password() const
{
    return ui->lineEditPassword->text();
}

void AuthenticationDialog::setPassword(const QString &password)
{
    ui->lineEditPassword->setText(password);
}
