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
    ui->lineEditStreet->setText(userInfo.street);
    ui->lineEditCity->setText(userInfo.city);
    ui->lineEditEmployedSince->setText(userInfo.employedSince.toString(tr("dd.MM.yyyy")));
    ui->lineEditEmployedTill->setText(userInfo.employedTill.toString(tr("dd.MM.yyyy")));
    ui->lineEditPlaceOfBirth->setText(userInfo.placeOfBirth);
    ui->lineEditZipcode->setText(userInfo.zipcode);
    ui->lineEditReligion->setText(userInfo.religion);
    ui->lineEditDepartment->setText(userInfo.department);
    ui->lineEditVerwendgr->setText(userInfo.verwendgr);
    ui->lineEditTaetig->setText(userInfo.taetig);
    ui->lineEditArbverh->setText(userInfo.arbverh);
    ui->lineEditBetriebsnr->setText(userInfo.betriebsnr);
}

AboutMeDialog::~AboutMeDialog()
{
    delete ui;
}
