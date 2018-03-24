#ifndef UPDATERSETTINGSWIDGET_H
#define UPDATERSETTINGSWIDGET_H

#include "settingswidget.h"

#include "updatersettings.h"

class QLineEdit;

class UpdaterSettingsWidget : public SettingsWidget
{
    Q_OBJECT

public:
    explicit UpdaterSettingsWidget(ZeiterfassungSettings &settings, QWidget *parent = Q_NULLPTR);

    virtual bool isValid(QString &message) const Q_DECL_OVERRIDE;

public Q_SLOTS:
    virtual void apply() Q_DECL_OVERRIDE;

private:
    UpdaterSettings m_settings;

    QLineEdit *m_lineEdit;
};

#endif // UPDATERSETTINGSWIDGET_H
