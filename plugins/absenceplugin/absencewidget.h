#pragma once

#include <QToolButton>

class AbsenceWidget : public QToolButton
{
    Q_OBJECT

public:
    explicit AbsenceWidget(QWidget *parent = Q_NULLPTR);

private Q_SLOTS:
    void pressedSlot();
};
