#include "presencewidget.h"

#include <QLabel>
#include <QStatusBar>
#include <QMenu>
#include <QTimer>
#include <QMessageBox>
#include <QStringBuilder>

#include "mainwindow.h"
#include "zeiterfassungapi.h"

PresenceWidget::PresenceWidget(MainWindow &mainWindow) :
    QWidget(&mainWindow),
    m_mainWindow(mainWindow)
{
    m_labelAvailable = new QLabel(this);
    m_labelAvailable->setFrameShape(QFrame::Panel);
    m_labelAvailable->setFrameShadow(QFrame::Sunken);
    m_mainWindow.statusBar()->addWidget(m_labelAvailable);

    m_labelNotAvailable = new QLabel(this);
    m_labelNotAvailable->setFrameShape(QFrame::Panel);
    m_labelNotAvailable->setFrameShadow(QFrame::Sunken);
    m_mainWindow.statusBar()->addWidget(m_labelNotAvailable);

    m_action = m_mainWindow.menuView()->addAction(QIcon(QStringLiteral(":zeiterfassung/plugins/presenceplugin/images/refresh.png")),
                                                  tr("Refresh presence"), this, &PresenceWidget::timeout);

    auto timer = new QTimer(this);
    timer->setInterval(60000);
    connect(timer, &QTimer::timeout, this, &PresenceWidget::timeout);
    timer->start();

    timeout();
}

void PresenceWidget::timeout()
{
    m_labelAvailable->setText(tr("%0: %1").arg(tr("Available")).arg(tr("???")));
    m_labelNotAvailable->setText(tr("%0: %1").arg(tr("Not available")).arg(tr("???")));

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
        int available = 0,
            notAvailable = 0;
        for(const auto &status : m_reply->presenceStatuses())
        {
            if(status.presence == QStringLiteral("J"))
                available++;
            else if(status.presence == QStringLiteral("N"))
                notAvailable++;
            else
                qWarning() << "unknown presence" << status.firstName << status.lastName << status.presence;
        }

        m_labelAvailable->setText(tr("%0: %1").arg(tr("Available")).arg(available));
        m_labelNotAvailable->setText(tr("%0: %1").arg(tr("Not available")).arg(notAvailable));
    }

    after:
    m_action->setEnabled(true);
    m_reply = Q_NULLPTR;
}
