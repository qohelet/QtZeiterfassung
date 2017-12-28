#pragma once

#include <QToolButton>

class StripsWidget;

class AbsenceWidget : public QToolButton
{
    Q_OBJECT

public:
    explicit AbsenceWidget(StripsWidget &stripsWidget);

private Q_SLOTS:
    void pressedSlot();

private:
    StripsWidget &m_stripsWidget;
};
