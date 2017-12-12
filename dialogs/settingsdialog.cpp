#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QMessageBox>
#include <QStringBuilder>
#include <QDir>
#include <QApplication>
#include <QFile>
#include <QTextStream>

#include "zeiterfassungsettings.h"

SettingsDialog::SettingsDialog(ZeiterfassungSettings &settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    m_settings(settings)
{
    ui->setupUi(this);

    ui->comboBoxLanguage->addItem(tr("English"), QLocale::English);
    ui->comboBoxLanguage->addItem(tr("German"), QLocale::German);

    {
        auto index = ui->comboBoxLanguage->findData(settings.language());
        if(index == -1)
            QMessageBox::warning(this, tr("Invalid settings!"), tr("Invalid settings!") % "\n\n" % tr("Unknown language!"));
        ui->comboBoxLanguage->setCurrentIndex(index);
    }

    ui->comboBoxTheme->addItem(tr("Default"), QString());

    for(const auto &entry : QDir(QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(QStringLiteral("themes"))).entryInfoList(QStringList { QStringLiteral("*.qss") }, QDir::Files))
        ui->comboBoxTheme->addItem(entry.baseName(), entry.baseName());

    if(!settings.theme().isEmpty())
    {
        auto index = ui->comboBoxTheme->findData(settings.theme());
        if(index == -1)
            QMessageBox::warning(this, tr("Invalid settings!"), tr("Invalid settings!") % "\n\n" % tr("Unknown theme!"));
        ui->comboBoxTheme->setCurrentIndex(index);
    }

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::submit);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::submit()
{
    auto warning = false;

    if(ui->comboBoxLanguage->currentIndex() == -1 ||
       ui->comboBoxTheme->currentIndex() == -1)
    {
        QMessageBox::warning(this, tr("Invalid settings!"), tr("Invalid settings!") % "\n\n" % tr("Please fill all options with valid values!"));
        return;
    }

    if(ui->comboBoxLanguage->currentData().value<QLocale::Language>() != m_settings.language())
    {
        m_settings.setLanguage(ui->comboBoxLanguage->currentData().value<QLocale::Language>());
        warning = true;
    }

    auto theme = ui->comboBoxTheme->currentData().toString();
    if(theme != m_settings.theme())
    {
        if(theme.isEmpty())
            qApp->setStyleSheet(QString());
        else
        {
            auto themePath = QDir(QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(QStringLiteral("themes"))).absoluteFilePath(theme);

            QFile file(themePath % ".qss");

            if(!file.exists())
            {
                QMessageBox::warning(this, tr("Could not load theme!"), tr("Could not load theme!") % "\n\n" % tr("Theme file does not exist!"));
                return;
            }

            if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QMessageBox::warning(this, tr("Could not load theme!"), tr("Could not load theme!") % "\n\n" % file.errorString());
                return;
            }

            QTextStream textStream(&file);
            qApp->setStyleSheet(textStream.readAll().replace(QStringLiteral("@THEME_RESOURCES@"), themePath));
        }

        m_settings.setTheme(theme);
    }

    if(warning)
        QMessageBox::information(this, tr("Restart required!"), tr("To apply the new settings a restart is required!"));

    accept();
}
