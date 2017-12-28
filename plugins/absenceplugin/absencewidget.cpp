#include "absencewidget.h"

#include "absencedialog.h"

AbsenceWidget::AbsenceWidget(QWidget *parent) :
    QToolButton(parent)
{
    setIcon(QIcon(QStringLiteral(":/zeiterfassung/plugins/absenceplugin/images/absence.png")));
    setText(tr("Absence"));

    connect(this, &QAbstractButton::pressed, this, &AbsenceWidget::pressedSlot);
}

void AbsenceWidget::pressedSlot()
{
    AbsenceDialog dialog(this);
    dialog.exec();
}
