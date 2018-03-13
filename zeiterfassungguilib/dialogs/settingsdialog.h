#pragma once

#include "zeiterfassungguilib_global.h"
#include "zeiterfassungdialog.h"

namespace Ui { class SettingsDialog; }
class ZeiterfassungSettings;
class ZeiterfassungPlugin;
class SettingsWidget;

class ZEITERFASSUNGGUILIBSHARED_EXPORT SettingsDialog : public ZeiterfassungDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(ZeiterfassungSettings &settings, const QSet<ZeiterfassungPlugin*> &plugins, QWidget *parent = Q_NULLPTR);
    ~SettingsDialog();

private Q_SLOTS:
    void submit();

private:
    Ui::SettingsDialog *ui;
    ZeiterfassungSettings &m_settings;
    QVector<SettingsWidget*> m_settingsWidgets;
};
