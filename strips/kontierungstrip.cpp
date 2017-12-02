#include "kontierungstrip.h"
#include "ui_kontierungstrip.h"

#include <QTime>
#include <QStringBuilder>

#include "mainwindow.h"

KontierungStrip::KontierungStrip(const QTime &startTime, const QTime &endTime, const QString &projekt,
                                 const QString &subprojekt, const QString &workpackage, const QString &text,
                                 QWidget *parent) :
    QFrame(parent),
    ui(new Ui::KontierungStrip)
{
    ui->setupUi(this);

    setStyleSheet("background-color: #7FFFFF;");

    if(endTime == QTime(0, 0))
    {
        ui->labelTime->setText(QStringLiteral("%0 - ???").arg(startTime.toString("HH:mm")));
        ui->labelDuration->setText(QStringLiteral("(???h)"));
    }
    else
    {
        ui->labelTime->setText(startTime.toString("HH:mm") % " - " % endTime.toString("HH:mm"));
        ui->labelDuration->setText("(" % MainWindow::timeBetween(startTime, endTime).toString("HH:mm") % "h)");
    }

    ui->labelProjekt->setText(projekt);
    ui->labelSubprojekt->setText(subprojekt);
    ui->labelWorkpackage->setText(workpackage);
    ui->labelText->setText(text);
}

KontierungStrip::~KontierungStrip()
{
    delete ui;
}
