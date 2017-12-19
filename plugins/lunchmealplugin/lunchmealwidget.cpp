#include "lunchmealwidget.h"

#include <QIcon>

#include "stripswidget.h"
#include "mainwindow.h"
#include "zeiterfassungapi.h"

#include "lunchmealdialog.h"

LunchMealWidget::LunchMealWidget(StripsWidget &stripsWidget) :
    QToolButton(&stripsWidget),
    m_stripsWidget(stripsWidget)
{
    setIcon(QIcon(QStringLiteral(":/zeiterfassung/plugins/lunchmealplugin/images/lunch-meal.png")));
    setText(tr("Lunch meal"));

    connect(this, &QAbstractButton::pressed, this, &LunchMealWidget::pressedSlot);
}

void LunchMealWidget::pressedSlot()
{
    LunchMealDialog dialog(m_stripsWidget);
    dialog.exec();
}
