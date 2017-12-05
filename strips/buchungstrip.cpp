#include "buchungstrip.h"
#include "ui_buchungstrip.h"

#include <QTime>
#include <QStringBuilder>

#include "zeiterfassungsettings.h"

BuchungStrip::BuchungStrip(int id, const QTime &time, const QString &type, const ZeiterfassungSettings &settings, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::BuchungStrip)
{
    ui->setupUi(this);

    setMinimumHeight(minimumSizeHint().height());
    setMaximumHeight(minimumSizeHint().height());

    ui->labelTime->setText(time.toString(QStringLiteral("HH:mm")));
    if(type == QStringLiteral("K"))
    {
        setStyleSheet(QStringLiteral("%0 { background-color: %1; }").arg(staticMetaObject.className()).arg(settings.buchungStartBackgroundColor()));
        ui->labelType->setText(tr("START"));
    }
    else if(type == QStringLiteral("G"))
    {
        setStyleSheet(QStringLiteral("%0 { background-color: %1; }").arg(staticMetaObject.className()).arg(settings.buchungEndBackgroundColor()));
        ui->labelType->setText(tr("END"));
    }
    else
    {
        setStyleSheet(QStringLiteral("%0 { background-color: %1; }").arg(staticMetaObject.className()).arg(settings.buchungOtherBackgroundColor()));
        ui->labelType->setText(tr("UNKNOWN"));
    }

    ui->labelId->setText(QString::number(id));
}

BuchungStrip::~BuchungStrip()
{
    delete ui;
}
