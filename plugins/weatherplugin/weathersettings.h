#ifndef WEATHERSETTINGS_H
#define WEATHERSETTINGS_H

#include <QWidget>

#include "settingswidget.h"

class WeatherSettings : public SettingsWidget
{
    Q_OBJECT
public:
    explicit WeatherSettings(QWidget *parent = Q_NULLPTR);

public Q_SLOTS:
    void apply() Q_DECL_OVERRIDE;
};

#endif // WEATHERSETTINGS_H
