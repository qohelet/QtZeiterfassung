#include "kontierungstrip.h"
#include "ui_kontierungstrip.h"

#include <QTime>
#include <QStringBuilder>

#include "zeiterfassungsettings.h"

KontierungStrip::KontierungStrip(int id, const QTime &duration, const QString &projekt, const QString &subprojekt,
                                 const QString &workpackage, const QString &text, const ZeiterfassungSettings &settings,
                                 QWidget *parent) :
    QFrame(parent),
    ui(new Ui::KontierungStrip)
{
    ui->setupUi(this);

    setMinimumHeight(minimumSizeHint().height());
    setMaximumHeight(minimumSizeHint().height());

    setStyleSheet(QStringLiteral("%0 { background-color: %1; }").arg(staticMetaObject.className()).arg(settings.kontierungBackgroundColor()));

    ui->labelTime->setText(duration == QTime(0, 0) ? tr("Open") : duration.toString(QStringLiteral("HH:mm")));
    ui->labelProjekt->setText(projekt);
    ui->labelId->setText(QString::number(id));

    ui->labelSubprojekt->setText(subprojekt);
    ui->labelWorkpackage->setText(workpackage);
    ui->labelText->setText(text);
}

KontierungStrip::~KontierungStrip()
{
    delete ui;
}
