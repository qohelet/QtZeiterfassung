#include "bookingstrip.h"
#include "ui_bookingstrip.h"

#include <QTime>
#include <QStringBuilder>

#include "zeiterfassungsettings.h"

BookingStrip::BookingStrip(int id, const QTime &time, const QString &type, const ZeiterfassungSettings &settings, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::BookingStrip)
{
    ui->setupUi(this);

    setMinimumHeight(minimumSizeHint().height());
    setMaximumHeight(minimumSizeHint().height());

    ui->labelTime->setText(time.toString(QStringLiteral("HH:mm")));
    if(type == QStringLiteral("K"))
    {
        setStyleSheet(QStringLiteral("%0 { background-color: %1; }").arg(staticMetaObject.className()).arg(settings.bookingStartBackgroundColor()));
        ui->labelType->setText(tr("START"));
    }
    else if(type == QStringLiteral("G"))
    {
        setStyleSheet(QStringLiteral("%0 { background-color: %1; }").arg(staticMetaObject.className()).arg(settings.bookingEndBackgroundColor()));
        ui->labelType->setText(tr("END"));
    }
    else
    {
        setStyleSheet(QStringLiteral("%0 { background-color: %1; }").arg(staticMetaObject.className()).arg(settings.bookingOtherBackgroundColor()));
        ui->labelType->setText(tr("UNKNOWN"));
    }

    ui->labelId->setText(QString::number(id));
}

BookingStrip::~BookingStrip()
{
    delete ui;
}
