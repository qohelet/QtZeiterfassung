#include "languageselectiondialog.h"
#include "ui_languageselectiondialog.h"

LanguageSelectionDialog::LanguageSelectionDialog(QWidget *parent) :
    ZeiterfassungDialog(parent),
    ui(new Ui::LanguageSelectionDialog)
{
    ui->setupUi(this);

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
