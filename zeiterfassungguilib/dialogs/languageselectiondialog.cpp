#include "languageselectiondialog.h"
#include "ui_languageselectiondialog.h"

LanguageSelectionDialog::LanguageSelectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LanguageSelectionDialog)
{
    ui->setupUi(this);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
#else
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
#endif

    ui->comboBoxLanguage->addItem(tr("English"), QLocale::English);
    ui->comboBoxLanguage->addItem(tr("German"), QLocale::German);
}

LanguageSelectionDialog::~LanguageSelectionDialog()
{
    delete ui;
}

QLocale::Language LanguageSelectionDialog::language() const
{
    return ui->comboBoxLanguage->currentData().value<QLocale::Language>();
}

void LanguageSelectionDialog::setLanguage(QLocale::Language language)
{
    ui->comboBoxLanguage->setCurrentIndex(ui->comboBoxLanguage->findData(language));
}
