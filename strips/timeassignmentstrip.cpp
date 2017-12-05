#include "timeassignmentstrip.h"
#include "ui_timeassignmentstrip.h"

#include <QTime>
#include <QStringBuilder>

#include "zeiterfassungsettings.h"

TimeAssignmentStrip::TimeAssignmentStrip(int id, const QTime &duration, const QString &projekt, const QString &subprojekt,
                                         const QString &workpackage, const QString &text, const ZeiterfassungSettings &settings,
                                         QWidget *parent) :
    QFrame(parent),
    ui(new Ui::TimeAssignmentStrip)
{
    ui->setupUi(this);

    setMinimumHeight(minimumSizeHint().height());
    setMaximumHeight(minimumSizeHint().height());

    setStyleSheet(QStringLiteral("%0 { background-color: %1; }").arg(staticMetaObject.className()).arg(settings.timeAssignmentBackgroundColor()));

    ui->labelTime->setText(duration == QTime(0, 0) ? tr("Open") : duration.toString(QStringLiteral("HH:mm")));
    ui->labelProjekt->setText(projekt);
    ui->labelId->setText(QString::number(id));

    ui->labelSubprojekt->setText(subprojekt);
    ui->labelWorkpackage->setText(workpackage);
    ui->labelText->setText(text);
}

TimeAssignmentStrip::~TimeAssignmentStrip()
{
    delete ui;
}
