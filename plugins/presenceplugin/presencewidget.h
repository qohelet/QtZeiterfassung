#ifndef PRESENCEWIDGET_H
#define PRESENCEWIDGET_H

#include <QWidget>

#include "replies/getpresencestatusreply.h"

class QLabel;
class QAction;

class MainWindow;

class PresenceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PresenceWidget(MainWindow &mainWindow);

private Q_SLOTS:
    void refresh();
    void finished();

private:
    MainWindow &m_mainWindow;

    QLabel *m_labelAvailable;
    QLabel *m_labelNotAvailable;

    QAction *m_action;

    std::unique_ptr<GetPresenceStatusReply> m_reply;
};

#endif // PRESENCEWIDGET_H
