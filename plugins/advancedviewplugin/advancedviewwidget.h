#ifndef ADVANCEDVIEWWIDGET_H
#define ADVANCEDVIEWWIDGET_H

#include <QPushButton>

class StripsWidget;

class AdvancedViewWidget : public QPushButton
{
    Q_OBJECT

public:
    explicit AdvancedViewWidget(StripsWidget &stripsWidget);

private Q_SLOTS:
    void dateChanged(const QDate &date);
    void pressedSlot();

private:
    StripsWidget &m_stripsWidget;
};

#endif // ADVANCEDVIEWWIDGET_H
