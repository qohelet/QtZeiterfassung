#pragma once

#include <QToolButton>

class StripsWidget;

class AdvancedViewWidget : public QToolButton
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
