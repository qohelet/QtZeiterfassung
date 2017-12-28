#include "absencewidget.h"

AbsenceWidget::AbsenceWidget(QWidget *parent) :
    QToolButton(parent)
{
    setIcon(QIcon(QStringLiteral(":/zeiterfassung/plugins/absenceplugin/images/absence.png")));
    setText(tr("Absence"));
}
