#include "absencedialog.h"
#include "ui_absencedialog.h"

#include <QDate>

#include "absencesmodel.h"

AbsenceDialog::AbsenceDialog(int userId, const QDate &date, ZeiterfassungApi &erfassung, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AbsenceDialog)
{
    ui->setupUi(this);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
#else
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
#endif

    ui->labelTitle->setText(tr("Absences for %0").arg(date.toString(tr("dd.MM.yyyy"))));

    ui->treeView->setModel(new AbsencesModel(userId, date, erfassung, this));
}

AbsenceDialog::~AbsenceDialog()
{
    delete ui;
}
