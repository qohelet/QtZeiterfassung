#include "advancedviewwidget.h"

#include <QIcon>

#include "stripswidget.h"

AdvancedViewWidget::AdvancedViewWidget(StripsWidget &stripsWidget) :
    QToolButton(&stripsWidget),
    m_stripsWidget(stripsWidget)
{
    setIcon(QIcon(QStringLiteral(":/zeiterfassunglib/plugins/advancedviewplugin/images/advanced-view.png")));
    setText(tr("Advanced view"));
    connect(&stripsWidget, &StripsWidget::dateChanged, this, &AdvancedViewWidget::dateChanged);
    dateChanged(stripsWidget.date());
}

void AdvancedViewWidget::dateChanged(const QDate &date)
{
    setEnabled(date.isValid());
}
