#include "buchungdialog.h"
#include "ui_buchungdialog.h"

BuchungDialog::BuchungDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BuchungDialog)
{
    ui->setupUi(this);
    ui->timeEditTime->setTime(QTime::currentTime());
}

BuchungDialog::~BuchungDialog()
{
    delete ui;
}

QTime BuchungDialog::getTime() const
{
    return ui->timeEditTime->time();
}

void BuchungDialog::setTime(const QTime &time)
{
    ui->timeEditTime->setTime(time);
}

QTime BuchungDialog::getTimespan() const
{
    return ui->timeEditTimespan->time();
}

void BuchungDialog::setTimespan(const QTime &timespan)
{
    ui->timeEditTimespan->setTime(timespan);
}

QString BuchungDialog::getType() const
{
    return ui->comboBoxType->currentText();
}

void BuchungDialog::setType(const QString &type)
{
    ui->comboBoxType->setCurrentText(type);
}

QString BuchungDialog::getText() const
{
    return ui->lineEditText->text();
}

void BuchungDialog::setText(const QString &text)
{
    ui->lineEditText->setText(text);
}
