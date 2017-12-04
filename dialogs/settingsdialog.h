#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class ZeiterfassungSettings;
namespace Ui { class SettingsDialog; }

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(ZeiterfassungSettings &settings, QWidget *parent = 0);
    ~SettingsDialog();

private Q_SLOTS:
    void submit();

private:
    Ui::SettingsDialog *ui;
    ZeiterfassungSettings &m_settings;
};

#endif // SETTINGSDIALOG_H
