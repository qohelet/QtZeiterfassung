#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QObject>
#include <QWidget>

#include "zeiterfassungguilib_global.h"

class ZEITERFASSUNGGUILIBSHARED_EXPORT SettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsWidget(QWidget *parent = Q_NULLPTR);

public Q_SLOTS:
    virtual void apply() { }
};

#endif // SETTINGSWIDGET_H
