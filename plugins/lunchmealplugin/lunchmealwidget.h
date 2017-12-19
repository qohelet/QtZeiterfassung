#ifndef LUNCHMEALWIDGET_H
#define LUNCHMEALWIDGET_H

#include <QToolButton>

class StripsWidget;

class LunchMealWidget : public QToolButton
{
    Q_OBJECT
public:
    explicit LunchMealWidget(StripsWidget &stripsWidget);

private Q_SLOTS:
    void pressedSlot();

private:
    StripsWidget &m_stripsWidget;
};

#endif // LUNCHMEALWIDGET_H
