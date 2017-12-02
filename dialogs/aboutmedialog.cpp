#include "aboutmedialog.h"
#include "ui_aboutmedialog.h"

AboutMeDialog::AboutMeDialog(const Zeiterfassung::UserInfo &userInfo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutMeDialog),
    m_userInfo(userInfo)
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
