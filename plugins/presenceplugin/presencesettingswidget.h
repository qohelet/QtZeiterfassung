#ifndef PRESENCESETTINGSWIDGET_H
#define PRESENCESETTINGSWIDGET_H

#include "settingswidget.h"

#include "presencesettings.h"

class QSpinBox;

class PresenceSettingsWidget : public SettingsWidget
{
    Q_OBJECT

public:
    explicit PresenceSettingsWidget(ZeiterfassungSettings &settings, QWidget *parent = Q_NULLPTR);

    virtual bool isValid(QString &message) const Q_DECL_OVERRIDE;

public Q_SLOTS:
    virtual void apply() Q_DECL_OVERRIDE;

private:
    PresenceSettings m_settings;

    QSpinBox *m_spinBox;
};

#endif // PRESENCESETTINGSWIDGET_H
