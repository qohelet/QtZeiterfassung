#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "zeiterfassungsettings.h"

SettingsDialog::SettingsDialog(ZeiterfassungSettings &settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    m_settings(settings)
{
    ui->setupUi(this);

    ui->lineEditBuchungKommenBackgroundColor->setText(settings.buchungKommenBackgroundColor());
    ui->lineEditBuchungGehenBackgroundColor->setText(settings.buchungGehenBackgroundColor());
    ui->lineEditBuchungOtherBackgroundColor->setText(settings.buchungOtherBackgroundColor());
    ui->lineEditKontierungBackgroundColor->setText(settings.kontierungBackgroundColor());

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::submit);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::submit()
{
    if(ui->lineEditBuchungKommenBackgroundColor->text() != m_settings.buchungKommenBackgroundColor())
        m_settings.setBuchungKommenBackgroundColor(ui->lineEditBuchungKommenBackgroundColor->text());
    if(ui->lineEditBuchungGehenBackgroundColor->text() != m_settings.buchungGehenBackgroundColor())
        m_settings.setBuchungGehenBackgroundColor(ui->lineEditBuchungGehenBackgroundColor->text());
    if(ui->lineEditBuchungOtherBackgroundColor->text() != m_settings.buchungOtherBackgroundColor())
        m_settings.setBuchungOtherBackgroundColor(ui->lineEditBuchungOtherBackgroundColor->text());
    if(ui->lineEditKontierungBackgroundColor->text() != m_settings.kontierungBackgroundColor())
        m_settings.setKontierungBackgroundColor(ui->lineEditKontierungBackgroundColor->text());

    accept();
}