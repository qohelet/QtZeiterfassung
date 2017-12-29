#pragma once

#include "zeiterfassungguilib_global.h"
#include "zeiterfassungdialog.h"

class ZeiterfassungSettings;
namespace Ui { class SettingsDialog; }

class ZEITERFASSUNGGUILIBSHARED_EXPORT SettingsDialog : public ZeiterfassungDialog
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
