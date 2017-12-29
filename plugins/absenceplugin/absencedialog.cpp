#include "absencedialog.h"
#include "ui_absencedialog.h"

#include <QDate>
#include <QMessageBox>
#include <QStringBuilder>

#include "absencesmodel.h"

AbsenceDialog::AbsenceDialog(int userId, const QDate &date, ZeiterfassungApi &erfassung, QWidget *parent) :
    ZeiterfassungDialog(parent),
    ui(new Ui::AbsenceDialog)
{
    ui->setupUi(this);

    ui->labelTitle->setText(tr("Absences for %0").arg(date.toString(tr("dd.MM.yyyy"))));

    m_model = new AbsencesModel(userId, date, erfassung, this);
    connect(m_model, &AbsencesModel::errorOccured, this, &AbsenceDialog::errorOccured);

    ui->treeView->setModel(m_model);
    ui->treeView->setEnabled(m_model->enabled());
    connect(m_model, &AbsencesModel::enabledChanged, ui->treeView, &QWidget::setEnabled);
}

AbsenceDialog::~AbsenceDialog()
{
    delete ui;
}

void AbsenceDialog::errorOccured(const QString &message)
{
    QMessageBox::warning(this, tr("Could not load absences!"), tr("Could not load absences!") % "\n\n" % message);
}
