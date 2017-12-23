#include "lunchmealdialog.h"
#include "ui_lunchmealdialog.h"

#include <QDate>

LunchMealDialog::LunchMealDialog(const QDate &date, const QString &content, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LunchMealDialog)
{
    ui->setupUi(this);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
#else
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
#endif

    ui->labelTitle->setText(tr("Lunch meal for %0").arg(date.toString(tr("dd.MM.yyyy"))));
    ui->labelLunchMeal->setText(content);
}

LunchMealDialog::~LunchMealDialog()
{
    delete ui;
}
