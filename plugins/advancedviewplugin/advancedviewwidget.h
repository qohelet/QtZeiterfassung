#ifndef ADVANCEDVIEWWIDGET_H
#define ADVANCEDVIEWWIDGET_H

#include <QToolButton>

class StripsWidget;

class AdvancedViewWidget : public QToolButton
{
    Q_OBJECT

public:
    explicit AdvancedViewWidget(StripsWidget &stripsWidget);

private Q_SLOTS:
    void dateChanged(const QDate &date);

private:
    StripsWidget &m_stripsWidget;
};

#endif // ADVANCEDVIEWWIDGET_H
