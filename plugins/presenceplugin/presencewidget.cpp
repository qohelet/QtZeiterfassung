#include "presencewidget.h"

#include <QPushButton>
#include <QMenu>
#include <QStatusBar>
#include <QTimer>
#include <QMessageBox>
#include <QStringBuilder>

#include <QPushButton>

#include "mainwindow.h"
#include "zeiterfassungapi.h"

PresenceWidget::PresenceWidget(MainWindow &mainWindow) :
    QWidget(&mainWindow),
    m_mainWindow(mainWindow)
{
    connect(&m_mainWindow, &MainWindow::refreshEverything, this, &PresenceWidget::refresh);

    m_buttonAvailable = new QPushButton(this);
    m_menuAvailable = new QMenu(this);
    m_buttonAvailable->setMenu(m_menuAvailable);
    m_mainWindow.statusBar()->addWidget(m_buttonAvailable);

    m_buttonNotAvailable = new QPushButton(this);
    m_menuNotAvailable = new QMenu(this);
    m_buttonNotAvailable->setMenu(m_menuNotAvailable);
    m_mainWindow.statusBar()->addWidget(m_buttonNotAvailable);

    m_action = m_mainWindow.menuView()->addAction(QIcon(QStringLiteral(":zeiterfassung/plugins/presenceplugin/images/refresh.png")),
                                                  tr("Refresh presence"), this, &PresenceWidget::refresh);

    auto timer = new QTimer(this);
    timer->setInterval(60000);
    connect(timer, &QTimer::timeout, this, &PresenceWidget::refresh);
    timer->start();

    refresh();
}

void PresenceWidget::refresh()
{
    m_buttonAvailable->setText(tr("%0: %1").arg(tr("Available")).arg(tr("???")));
    m_menuAvailable->clear();
    m_buttonNotAvailable->setText(tr("%0: %1").arg(tr("Not available")).arg(tr("???")));
    m_menuNotAvailable->clear();

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

    for(const auto &status : m_reply->presenceStatuses())
    {
        if(status.presence == QStringLiteral("J"))
            m_menuAvailable->addAction(tr("%0 %1").arg(status.firstName).arg(status.lastName));
        else if(status.presence == QStringLiteral("N"))
            m_menuNotAvailable->addAction(tr("%0 %1").arg(status.firstName).arg(status.lastName));
        else
            qWarning() << "unknown presence" << status.firstName << status.lastName << status.presence;
    }

    m_buttonAvailable->setText(tr("%0: %1").arg(tr("Available")).arg(m_menuAvailable->actions().count()));
    m_buttonNotAvailable->setText(tr("%0: %1").arg(tr("Not available")).arg(m_menuNotAvailable->actions().count()));

    after:
    m_action->setEnabled(true);
    m_reply = Q_NULLPTR;
}
