#ifndef WEATHERSETTINGS_H
#define WEATHERSETTINGS_H

#include <QWidget>

#include "settingswidget.h"

class QLineEdit;

class ZeiterfassungSettings;

class WeatherSettingsWidget : public SettingsWidget
{
    Q_OBJECT
public:
    explicit WeatherSettingsWidget(ZeiterfassungSettings &settings, QWidget *parent = Q_NULLPTR);

    bool isValid(QString &message) const Q_DECL_OVERRIDE;

public Q_SLOTS:
    void apply() Q_DECL_OVERRIDE;

private:
    ZeiterfassungSettings &m_settings;

    QLineEdit *m_lineEdit;
};

#endif // WEATHERSETTINGS_H
