#include "absencedialog.h"
#include "ui_absencedialog.h"

#include <QDate>

AbsenceDialog::AbsenceDialog(const QDate &date, QWidget *parent) :
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
}

AbsenceDialog::~AbsenceDialog()
{
    delete ui;
}
