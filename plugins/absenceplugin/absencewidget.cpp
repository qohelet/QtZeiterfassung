#include "absencewidget.h"

#include "stripswidget.h"

#include "absencedialog.h"

AbsenceWidget::AbsenceWidget(StripsWidget &stripsWidget) :
    QToolButton(&stripsWidget),
    m_stripsWidget(stripsWidget)
{
    setIcon(QIcon(QStringLiteral(":/zeiterfassung/plugins/absenceplugin/images/absence.png")));
    setText(tr("Absence"));

    connect(this, &QAbstractButton::pressed, this, &AbsenceWidget::pressedSlot);
}

void AbsenceWidget::pressedSlot()
{
    AbsenceDialog dialog(m_stripsWidget.date(), this);
    dialog.exec();
}
