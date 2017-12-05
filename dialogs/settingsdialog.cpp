#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QMessageBox>

#include "zeiterfassungsettings.h"

SettingsDialog::SettingsDialog(ZeiterfassungSettings &settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    m_settings(settings)
{
    ui->setupUi(this);

    ui->comboBoxLanguage->addItem(tr("English"), QLocale::English);
    ui->comboBoxLanguage->addItem(tr("German"), QLocale::German);

    ui->comboBoxLanguage->setCurrentIndex(ui->comboBoxLanguage->findData(settings.language()));
    ui->lineEditBookingStartBackgroundColor->setText(settings.bookingStartBackgroundColor());
    ui->lineEditBookingEndBackgroundColor->setText(settings.bookingEndBackgroundColor());
    ui->lineEditBookingOtherBackgroundColor->setText(settings.bookingOtherBackgroundColor());
    ui->lineEditTimeAssignmentBackgroundColor->setText(settings.timeAssignmentBackgroundColor());

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::submit);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::submit()
{
    if(ui->comboBoxLanguage->currentData().value<QLocale::Language>() != m_settings.language())
    {
        m_settings.setLanguage(ui->comboBoxLanguage->currentData().value<QLocale::Language>());
        QMessageBox::warning(this, tr("Restart required!"), tr("To apply the new language a restart is required!"));
    }
    if(ui->lineEditBookingStartBackgroundColor->text() != m_settings.bookingStartBackgroundColor())
        m_settings.setBookingStartBackgroundColor(ui->lineEditBookingStartBackgroundColor->text());
    if(ui->lineEditBookingEndBackgroundColor->text() != m_settings.bookingEndBackgroundColor())
        m_settings.setBookingEndBackgroundColor(ui->lineEditBookingEndBackgroundColor->text());
    if(ui->lineEditBookingOtherBackgroundColor->text() != m_settings.bookingOtherBackgroundColor())
        m_settings.setBookingOtherBackgroundColor(ui->lineEditBookingOtherBackgroundColor->text());
    if(ui->lineEditTimeAssignmentBackgroundColor->text() != m_settings.timeAssignmentBackgroundColor())
        m_settings.setTimeAssignmentBackgroundColor(ui->lineEditTimeAssignmentBackgroundColor->text());

    accept();
}
