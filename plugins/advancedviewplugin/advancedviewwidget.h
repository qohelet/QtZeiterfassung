#ifndef ADVANCEDVIEWWIDGET_H
#define ADVANCEDVIEWWIDGET_H

#include <QToolButton>

class MainWindow;

class AdvancedViewWidget : public QToolButton
{
    Q_OBJECT

public:
    explicit AdvancedViewWidget(MainWindow &mainWindow);

private:
    MainWindow &m_mainWindow;
};

#endif // ADVANCEDVIEWWIDGET_H
