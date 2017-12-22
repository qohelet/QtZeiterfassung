#pragma once

#include <QWidget>

#include "replies/getpresencestatusreply.h"

class QPushButton;
class QMenu;
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

    QPushButton *m_buttonAvailable;
    QMenu *m_menuAvailable;
    QPushButton *m_buttonNotAvailable;
    QMenu *m_menuNotAvailable;

    QAction *m_action;

    std::unique_ptr<GetPresenceStatusReply> m_reply;
};
