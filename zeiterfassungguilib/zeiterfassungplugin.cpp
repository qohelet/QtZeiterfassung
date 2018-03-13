#include "zeiterfassungplugin.h"

ZeiterfassungPlugin::ZeiterfassungPlugin(QObject *parent) :
    QObject(parent)
{
}

SettingsWidget *ZeiterfassungPlugin::settingsWidget(ZeiterfassungSettings &settings, QWidget *parent) const
{
    Q_UNUSED(settings)
    Q_UNUSED(parent)
    return Q_NULLPTR;
}
