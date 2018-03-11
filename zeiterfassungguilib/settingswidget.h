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

    virtual bool isValid(QString &message) const { Q_UNUSED(message) return true; }

public Q_SLOTS:
    virtual void apply() { }
};

#endif // SETTINGSWIDGET_H
