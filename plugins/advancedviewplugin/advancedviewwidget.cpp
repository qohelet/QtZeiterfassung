#include "advancedviewwidget.h"

#include <QIcon>

#include "stripswidget.h"
#include "advancedviewdialog.h"

AdvancedViewWidget::AdvancedViewWidget(StripsWidget &stripsWidget) :
    QPushButton(&stripsWidget),
    m_stripsWidget(stripsWidget)
{
    setIcon(QIcon(QStringLiteral(":/zeiterfassung/plugins/advancedviewplugin/images/advanced-view.png")));

    connect(&stripsWidget, &StripsWidget::dateChanged, this, &AdvancedViewWidget::dateChanged);
    dateChanged(stripsWidget.date());

    connect(this, &QAbstractButton::pressed, this, &AdvancedViewWidget::pressedSlot);
}

void AdvancedViewWidget::dateChanged(const QDate &date)
{
    setEnabled(date.isValid());
}

void AdvancedViewWidget::pressedSlot()
{
    AdvancedViewDialog dialog(m_stripsWidget);
    dialog.exec();
}
