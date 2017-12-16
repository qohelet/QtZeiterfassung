#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

#include "zeiterfassunglib_global.h"

class ZeiterfassungSettings;
namespace Ui { class SettingsDialog; }

class ZEITERFASSUNGLIBSHARED_EXPORT SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(ZeiterfassungSettings &settings, QWidget *parent = Q_NULLPTR);
    ~SettingsDialog();

private Q_SLOTS:
    void submit();

private:
    Ui::SettingsDialog *ui;
    ZeiterfassungSettings &m_settings;
};

#endif // SETTINGSDIALOG_H
