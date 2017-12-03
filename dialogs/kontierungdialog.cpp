#include "kontierungdialog.h"
#include "ui_kontierungdialog.h"

#include <QMap>
#include <QStringBuilder>
#include <QDebug>

#include "zeiterfassungsettings.h"

KontierungDialog::KontierungDialog(const QMap<QString, QString> &projekte, const ZeiterfassungSettings &settings,
                                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KontierungDialog)
{
    ui->setupUi(this);

    {
        for(const auto &preferedProjekt : settings.projekte())
        {
            if(!projekte.contains(preferedProjekt))
            {
                qWarning() << "cannot find projekt" << preferedProjekt;
                continue;
            }

            ui->comboBoxProjekt->addItem(projekte.value(preferedProjekt) % " (" % preferedProjekt % ')', preferedProjekt);
        }

        if(settings.projekte().count())
            ui->comboBoxProjekt->insertSeparator(ui->comboBoxProjekt->count());

        for(auto iter = projekte.constBegin(); iter != projekte.constEnd(); iter++)
        {
            if(!settings.projekte().contains(iter.key()))
                ui->comboBoxProjekt->addItem(iter.value() % " (" % iter.key() % ')', iter.key());
        }
    }

    for(const auto &subprojekt : settings.subprojekte())
        ui->comboBoxSubprojekt->addItem(subprojekt);
    ui->comboBoxSubprojekt->clearEditText();

    for(const auto &workpackage : settings.workpackages())
        ui->comboBoxWorkpackage->addItem(workpackage);
    ui->comboBoxWorkpackage->clearEditText();

    for(const auto &text : settings.texte())
        ui->comboBoxText->addItem(text);
    ui->comboBoxText->clearEditText();
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
    return ui->comboBoxSubprojekt->currentText();
}

void KontierungDialog::setSubprojekt(const QString &subprojekt)
{
    ui->comboBoxSubprojekt->setCurrentText(subprojekt);
}

QString KontierungDialog::getWorkpackage() const
{
    return ui->comboBoxWorkpackage->currentText();
}

void KontierungDialog::setWorkpackage(const QString &workpackage)
{
    ui->comboBoxWorkpackage->setCurrentText(workpackage);
}

QString KontierungDialog::getText() const
{
    return ui->comboBoxText->currentText();
}

void KontierungDialog::setText(const QString &text)
{
    ui->comboBoxText->setCurrentText(text);
}
