#include "kontierungstrip.h"
#include "ui_kontierungstrip.h"

#include <QTime>
#include <QStringBuilder>

#include "mainwindow.h"

KontierungStrip::KontierungStrip(int id, const QTime &duration, const QString &projekt, const QString &subprojekt,
                                 const QString &workpackage, const QString &text, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::KontierungStrip)
{
    ui->setupUi(this);

    setMinimumHeight(minimumSizeHint().height());
    setMaximumHeight(minimumSizeHint().height());

    setStyleSheet("background-color: #7FFFFF;");

    ui->labelTime->setText(duration == QTime(0, 0) ? tr("Open") : duration.toString("HH:mm"));
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
