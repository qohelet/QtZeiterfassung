#include "timeassignmentstrip.h"
#include "ui_timeassignmentstrip.h"

#include <QTime>
#include <QStringBuilder>

#include "zeiterfassungsettings.h"

TimeAssignmentStrip::TimeAssignmentStrip(int id, const QTime &duration, const QString &project, const QString &subproject,
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
    ui->labelProject->setText(project);
    ui->labelId->setText(QString::number(id));

    ui->labelSubproject->setText(subproject);
    ui->labelWorkpackage->setText(workpackage);
    ui->labelText->setText(text);
}

TimeAssignmentStrip::~TimeAssignmentStrip()
{
    delete ui;
}
