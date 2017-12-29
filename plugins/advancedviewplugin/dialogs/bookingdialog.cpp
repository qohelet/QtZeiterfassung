#include "bookingdialog.h"
#include "ui_bookingdialog.h"

BookingDialog::BookingDialog(QWidget *parent) :
    ZeiterfassungDialog(parent),
    ui(new Ui::BookingDialog)
{
    ui->setupUi(this);
}

BookingDialog::~BookingDialog()
{
    delete ui;
}

QTime BookingDialog::getTime() const
{
    return ui->timeEditTime->time();
}

void BookingDialog::setTime(const QTime &time)
{
    ui->timeEditTime->setTime(time);
}

QTime BookingDialog::getTimespan() const
{
    return ui->timeEditTimespan->time();
}

void BookingDialog::setTimespan(const QTime &timespan)
{
    ui->timeEditTimespan->setTime(timespan);
}

QString BookingDialog::getType() const
{
    return ui->comboBoxType->currentText();
}

void BookingDialog::setType(const QString &type)
{
    ui->comboBoxType->setCurrentText(type);
}

QString BookingDialog::getText() const
{
    return ui->lineEditText->text();
}

void BookingDialog::setText(const QString &text)
{
    ui->lineEditText->setText(text);
}
