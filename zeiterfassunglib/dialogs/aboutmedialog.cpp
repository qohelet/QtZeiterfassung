#include "aboutmedialog.h"
#include "ui_aboutmedialog.h"

AboutMeDialog::AboutMeDialog(const GetUserInfoReply::UserInfo &userInfo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutMeDialog)
{
    ui->setupUi(this);

    ui->spinBoxUserId->setValue(userInfo.userId);
    ui->lineEditEmail->setText(userInfo.email);
    ui->lineEditLongUsername->setText(userInfo.longUsername);
    ui->lineEditText->setText(userInfo.text);
    ui->lineEditUsername->setText(userInfo.username);
}

AboutMeDialog::~AboutMeDialog()
{
    delete ui;
}
