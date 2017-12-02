#include "kontierungdialog.h"
#include "ui_kontierungdialog.h"

#include <QMap>
#include <QMessageBox>
#include <QStringBuilder>
#include <QDebug>

KontierungDialog::KontierungDialog(Zeiterfassung &erfassung, const Zeiterfassung::UserInfo &userInfo,
                                   const QMap<QString, QString> &projekte, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KontierungDialog),
    m_erfassung(erfassung),
    m_userInfo(userInfo)
{
    ui->setupUi(this);

    for(auto iter = projekte.constBegin(); iter != projekte.constEnd(); iter++)
        ui->comboBoxProjekt->addItem(iter.value() % " (" % iter.key() % ')', iter.key());
}

KontierungDialog::~KontierungDialog()
{
    delete ui;
}

QTime KontierungDialog::getTime() const
{
    return ui->timeEditTime->time();
}

void KontierungDialog::setTime(const QTime &time)
{
    ui->timeEditTime->setTime(time);
}

QTime KontierungDialog::getTimespan() const
{
    return ui->timeEditTimespan->time();
}

void KontierungDialog::setTimespan(const QTime &timespan)
{
    ui->timeEditTimespan->setTime(timespan);
}

QString KontierungDialog::getProjekt() const
{
    return ui->comboBoxProjekt->currentData().toString();
}

void KontierungDialog::setProjekt(const QString &projekt)
{
    auto index = ui->comboBoxProjekt->findData(projekt);
    if(index >= 0)
        ui->comboBoxProjekt->setCurrentIndex(index);
    else
        qWarning() << "could not find projekt" << projekt;
}

QString KontierungDialog::getSubprojekt() const
{
    return ui->lineEditSubprojekt->text();
}

void KontierungDialog::setSubprojekt(const QString &subprojekt)
{
    ui->lineEditSubprojekt->setText(subprojekt);
}

QString KontierungDialog::getWorkpackage() const
{
    return ui->lineEditWorkpackage->text();
}

void KontierungDialog::setWorkpackage(const QString &workpackage)
{
    ui->lineEditWorkpackage->setText(workpackage);
}

QString KontierungDialog::getText() const
{
    return ui->lineEditText->text();
}

void KontierungDialog::setText(const QString &text)
{
    ui->lineEditText->setText(text);
}
