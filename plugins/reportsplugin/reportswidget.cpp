#include "reportswidget.h"

#include <QLabel>
#include <QStatusBar>
#include <QMenu>
#include <QToolBar>
#include <QMessageBox>
#include <QStringBuilder>
#include <QRegularExpression>
#include <QTemporaryFile>
#include <QDir>
#include <QDesktopServices>

#include "mainwindow.h"
#include "zeiterfassungapi.h"

ReportsWidget::ReportsWidget(MainWindow &mainWindow) :
    QWidget(&mainWindow),
    m_mainWindow(mainWindow)
{
    m_labelBalance = new QLabel(this);
    m_labelBalance->setFrameShape(QFrame::Panel);
    m_labelBalance->setFrameShadow(QFrame::Sunken);
    m_mainWindow.statusBar()->addPermanentWidget(m_labelBalance);

    m_labelHolidays = new QLabel(this);
    m_labelHolidays->setFrameShape(QFrame::Panel);
    m_labelHolidays->setFrameShadow(QFrame::Sunken);
    m_mainWindow.statusBar()->addPermanentWidget(m_labelHolidays);

    m_actionRefreshReport = m_mainWindow.menuView()->addAction(QIcon(QStringLiteral(":/zeiterfassung/plugins/reportsplugin/images/refresh.png")),
                                                               tr("Refresh report"), this, &ReportsWidget::refresh);

    m_actionOpenReport = m_mainWindow.menuTools()->addAction(QIcon(QStringLiteral(":/zeiterfassung/plugins/reportsplugin/images/report.png")),
                                                             tr("Open report"), this, &ReportsWidget::openReport);
    m_mainWindow.toolBar()->addAction(m_actionOpenReport);

    dateChanged(m_mainWindow.date());
}

void ReportsWidget::dateChanged(const QDate &date)
{
    if(!date.isValid())
    {
        qWarning() << "invalid date" << date;
        return;
    }

    auto monthBegin = QDate(date.year(), date.month(), 1);
    if(monthBegin != m_date)
    {
        m_date = monthBegin;
        refresh();
    }
}

void ReportsWidget::refresh()
{
    if(!m_date.isValid())
    {
        qWarning() << "invalid date" << m_date;
        return;
    }

    m_actionRefreshReport->setEnabled(false);
    m_actionOpenReport->setEnabled(false);

    m_labelBalance->setText(tr("%0: %1").arg(tr("Balance")).arg(tr("???")));
    m_labelHolidays->setText(tr("%0: %1").arg(tr("Holidays")).arg(tr("???")));

    m_reply = m_mainWindow.erfassung().doGetAuswertung(m_mainWindow.userInfo().userId, m_date);
    connect(m_reply.get(), &ZeiterfassungReply::finished, this, &ReportsWidget::finished);
}

void ReportsWidget::finished()
{
    if(!m_reply->success())
    {
        m_date = QDate();
        QMessageBox::warning(this, tr("Could not load report!"), tr("Could not load report!") % "\n\n" % m_reply->message());
        goto after;
    }

    {
        auto content = m_reply->auswertung();

        {
            static QRegularExpression regex(QStringLiteral("Gleitzeit +([0-9]+\\:[0-9]+\\-?) +([0-9]+\\:[0-9]+\\-?)"));
            auto match = regex.match(content);
            if(match.hasMatch())
            {
                auto balance = match.captured(2);
                if(balance.endsWith(QChar('-')))
                {
                    balance.chop(1);
                    balance = QChar('-') % balance;
                }
                m_labelBalance->setText(tr("%0: %1").arg(tr("Balance")).arg(tr("%0h").arg(balance)));
            }
            else
            {
                m_labelBalance->setText(tr("%0: %1").arg(tr("Balance")).arg(tr("n/a")));
                qWarning() << "balance not found in PDF";
            }
        }

        {
            static QRegularExpression regex(QStringLiteral("Urlaubsanspruch +([0-9]+\\.[0-9]+\\-?) +([0-9]+\\.[0-9]+\\-?)"));
            auto match = regex.match(content);
            if(match.hasMatch())
                m_labelHolidays->setText(tr("%0: %1").arg(tr("Holidays")).arg(match.captured(2)));
            else
            {
                m_labelHolidays->setText(tr("%0: %1").arg(tr("Holidays")).arg(tr("n/a")));
                qWarning() << "holidays not found in PDF";
            }
        }

        {
            QTemporaryFile file(QDir::temp().absoluteFilePath(QStringLiteral("auswertungXXXXXX.pdf")));
            file.setAutoRemove(false);
            if(!file.open())
            {
                QMessageBox::warning(this, tr("Could not write report!"), tr("Could not write report!") % "\n\n" % file.errorString());
                goto after;
            }

            file.write(content);

            m_url = QUrl::fromLocalFile(file.fileName());
        }
    }

    m_actionOpenReport->setEnabled(true);

    after:
    m_actionRefreshReport->setEnabled(true);
    m_reply = Q_NULLPTR;
}

void ReportsWidget::openReport()
{
    if(!QDesktopServices::openUrl(m_url))
        QMessageBox::warning(this, tr("Could not launch your default PDF viewer!"), tr("Could not launch your default PDF viewer!"));
}
