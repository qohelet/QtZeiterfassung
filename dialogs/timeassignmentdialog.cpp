#include "timeassignmentdialog.h"
#include "ui_timeassignmentdialog.h"

#include <QMap>
#include <QStringBuilder>
#include <QDebug>

#include "zeiterfassungsettings.h"

TimeAssignmentDialog::TimeAssignmentDialog(const QMap<QString, QString> &projekte, const ZeiterfassungSettings &settings,
                                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimeAssignmentDialog)
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

TimeAssignmentDialog::~TimeAssignmentDialog()
{
    delete ui;
}

QTime TimeAssignmentDialog::getTime() const
{
    return ui->timeEditTime->time();
}

void TimeAssignmentDialog::setTime(const QTime &time)
{
    ui->timeEditTime->setTime(time);
}

QTime TimeAssignmentDialog::getTimespan() const
{
    return ui->timeEditTimespan->time();
}

void TimeAssignmentDialog::setTimespan(const QTime &timespan)
{
    ui->timeEditTimespan->setTime(timespan);
}

QString TimeAssignmentDialog::getProjekt() const
{
    return ui->comboBoxProjekt->currentData().toString();
}

void TimeAssignmentDialog::setProjekt(const QString &projekt)
{
    auto index = ui->comboBoxProjekt->findData(projekt);
    if(index >= 0)
        ui->comboBoxProjekt->setCurrentIndex(index);
    else
        qWarning() << "could not find projekt" << projekt;
}

QString TimeAssignmentDialog::getSubprojekt() const
{
    return ui->comboBoxSubprojekt->currentText();
}

void TimeAssignmentDialog::setSubprojekt(const QString &subprojekt)
{
    ui->comboBoxSubprojekt->setCurrentText(subprojekt);
}

QString TimeAssignmentDialog::getWorkpackage() const
{
    return ui->comboBoxWorkpackage->currentText();
}

void TimeAssignmentDialog::setWorkpackage(const QString &workpackage)
{
    ui->comboBoxWorkpackage->setCurrentText(workpackage);
}

QString TimeAssignmentDialog::getText() const
{
    return ui->comboBoxText->currentText();
}

void TimeAssignmentDialog::setText(const QString &text)
{
    ui->comboBoxText->setCurrentText(text);
}
