#include "timeassignmentdialog.h"
#include "ui_timeassignmentdialog.h"

#include <QMap>
#include <QStringBuilder>
#include <QDebug>

#include "zeiterfassungsettings.h"

TimeAssignmentDialog::TimeAssignmentDialog(const QMap<QString, QString> &projects, const ZeiterfassungSettings &settings,
                                   QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimeAssignmentDialog)
{
    ui->setupUi(this);

    {
        for(const auto &preferedProject : settings.projects())
        {
            if(!projects.contains(preferedProject))
            {
                qWarning() << "cannot find project" << preferedProject;
                continue;
            }

            ui->comboBoxProject->addItem(projects.value(preferedProject) % " (" % preferedProject % ')', preferedProject);
        }

        if(settings.projects().count())
            ui->comboBoxProject->insertSeparator(ui->comboBoxProject->count());

        for(auto iter = projects.constBegin(); iter != projects.constEnd(); iter++)
        {
            if(!settings.projects().contains(iter.key()))
                ui->comboBoxProject->addItem(iter.value() % " (" % iter.key() % ')', iter.key());
        }
    }

    for(const auto &subproject : settings.subprojects())
        ui->comboBoxSubproject->addItem(subproject);
    ui->comboBoxSubproject->clearEditText();

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

QString TimeAssignmentDialog::getProject() const
{
    return ui->comboBoxProject->currentData().toString();
}

void TimeAssignmentDialog::setProject(const QString &project)
{
    auto index = ui->comboBoxProject->findData(project);
    if(index >= 0)
        ui->comboBoxProject->setCurrentIndex(index);
    else
        qWarning() << "could not find project" << project;
}

QString TimeAssignmentDialog::getSubproject() const
{
    return ui->comboBoxSubproject->currentText();
}

void TimeAssignmentDialog::setSubproject(const QString &subproject)
{
    ui->comboBoxSubproject->setCurrentText(subproject);
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
