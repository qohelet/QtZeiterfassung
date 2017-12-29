#include "presencewidget.h"

#include <QMenu>
#include <QStatusBar>
#include <QTimer>
#include <QMessageBox>
#include <QStringBuilder>
#include <QDebug>

#include <QPushButton>

#include "mainwindow.h"
#include "zeiterfassungapi.h"

PresenceWidget::PresenceWidget(MainWindow &mainWindow) :
    QPushButton(&mainWindow),
    m_mainWindow(mainWindow)
{
    connect(&m_mainWindow, &MainWindow::refreshEverything, this, &PresenceWidget::refresh);

    m_menu = new QMenu(this);
    setMenu(m_menu);

    m_action = new QAction(QIcon(QStringLiteral(":zeiterfassung/plugins/presenceplugin/images/refresh.png")),
                                                  tr("Refresh presence"), this);
    connect(m_action, &QAction::triggered, this, &PresenceWidget::refresh);
    m_mainWindow.menuView()->addAction(m_action);

    auto timer = new QTimer(this);
    timer->setInterval(60000);
    connect(timer, &QTimer::timeout, this, &PresenceWidget::refresh);
    timer->start();

    refresh();
}

void PresenceWidget::refresh()
{
    setText(tr("%0 available, %1 not available").arg(tr("???")).arg(tr("???")));
    m_menu->clear();

    m_action->setEnabled(false);

    m_reply = m_mainWindow.erfassung().doGetPresenceStatus();
    connect(m_reply.get(), &ZeiterfassungReply::finished, this, &PresenceWidget::finished);
}

void PresenceWidget::finished()
{
    if(!m_reply->success())
    {
        QMessageBox::warning(&m_mainWindow, tr("Could not get presence status!"),
                             tr("Could not get presence status!") % "\n\n" % m_reply->message());
        goto after;
    }

    {
        int present = 0,
            notPresent = 0;

        for(const auto &status : m_reply->presenceStatuses())
        {
            QIcon icon;
            if(status.presence == QStringLiteral("J"))
            {
                present++;
                icon = QIcon(QStringLiteral(":zeiterfassung/plugins/presenceplugin/images/present.png"));
            }
            else if(status.presence == QStringLiteral("N"))
            {
                notPresent++;
                icon = QIcon(QStringLiteral(":zeiterfassung/plugins/presenceplugin/images/not-present.png"));
            }
            else
                qCritical() << "unknown presence" << status.firstName << status.lastName << status.presence;

            m_menu->addAction(icon, tr("%0 %1").arg(status.firstName).arg(status.lastName));
        }

        setText(tr("%0 available, %1 not available").arg(present).arg(notPresent));
    }

    after:
    m_action->setEnabled(true);
    m_reply = Q_NULLPTR;
}
