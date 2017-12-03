#include "buchungstrip.h"
#include "ui_buchungstrip.h"

#include <QTime>

BuchungStrip::BuchungStrip(int id, const QTime &time, const QString &type, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::BuchungStrip)
{
    ui->setupUi(this);

    setMinimumHeight(minimumSizeHint().height());
    setMaximumHeight(minimumSizeHint().height());

    ui->labelTime->setText(time.toString("HH:mm"));
    if(type == QStringLiteral("K"))
    {
        setStyleSheet("BuchungStrip { background-color: qlineargradient( x1:0 y1:0, x2:0 y2:1, stop:0 #7FFF7F, stop:1 #6FBF6F); }");
        ui->labelType->setText(tr("KOMMEN"));
    }
    else if(type == QStringLiteral("G"))
    {
        setStyleSheet("BuchungStrip { background-color: qlineargradient( x1:0 y1:0, x2:0 y2:1, stop:0 #FF7F7F, stop:1 #BF6F6F); }");
        ui->labelType->setText(tr("GEHEN"));
    }
    else
    {
        setStyleSheet("BuchungStrip { background-color: qlineargradient( x1:0 y1:0, x2:0 y2:1, stop:0 #FFFF7F, stop:1 #BFBF6F); }");
        ui->labelType->setText(tr("UNKNOWN"));
    }

    ui->labelId->setText(QString::number(id));
}

BuchungStrip::~BuchungStrip()
{
    delete ui;
}
