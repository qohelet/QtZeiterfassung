#pragma once

#include <QPushButton>

#include "replies/getpresencestatusreply.h"

class QMenu;
class QAction;

class MainWindow;

class PresenceWidget : public QPushButton
{
    Q_OBJECT

public:
    explicit PresenceWidget(MainWindow &mainWindow);

private Q_SLOTS:
    void refresh();
    void finished();

private:
    MainWindow &m_mainWindow;

    QMenu *m_menu;
    QAction *m_action;

    std::unique_ptr<GetPresenceStatusReply> m_reply;
};
