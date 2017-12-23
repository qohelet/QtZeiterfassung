#include "reportswidget.h"

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
    QLabel(&mainWindow),
    m_mainWindow(mainWindow)
{
    connect(&m_mainWindow, &MainWindow::dateChanged, this, &ReportsWidget::dateChanged);
    connect(&m_mainWindow, &MainWindow::refreshEverything, this, &ReportsWidget::refresh);

    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Sunken);

    m_actionRefreshReport = new QAction(QIcon(QStringLiteral(":/zeiterfassung/plugins/reportsplugin/images/refresh.png")),
                                        tr("Refresh report"), this);
    connect(m_actionRefreshReport, &QAction::triggered, this, &ReportsWidget::refresh);
    m_mainWindow.menuView()->addAction(m_actionRefreshReport);

    m_actionOpenReport = new QAction(QIcon(QStringLiteral(":/zeiterfassung/plugins/reportsplugin/images/report.png")),
                                     tr("Open report"), this);
    connect(m_actionOpenReport, &QAction::triggered, this, &ReportsWidget::openReport);
    m_mainWindow.menuTools()->addAction(m_actionOpenReport);

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

    setText(tr("Balance: %0, Holidays: %1").arg(tr("???")).arg(tr("???")));

    m_actionRefreshReport->setEnabled(false);
    m_actionOpenReport->setEnabled(false);

    m_reply = m_mainWindow.erfassung().doGetReport(m_mainWindow.userInfo().userId, m_date);
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
        auto content = m_reply->content();

        QString balance;

        {
            static QRegularExpression regex(QStringLiteral("Gleitzeit +([0-9]+\\:[0-9]+\\-?) +([0-9]+\\:[0-9]+\\-?)"));
            auto match = regex.match(content);
            if(match.hasMatch())
            {
                balance = match.captured(2);
                if(balance.endsWith(QChar('-')))
                {
                    balance.chop(1);
                    balance = QChar('-') % balance;
                }
            }
            else
            {
                balance = tr("n/a");
                qWarning() << "balance not found in PDF";
            }
        }

        QString holidays;

        {
            static QRegularExpression regex(QStringLiteral("Urlaubsanspruch +(\\-?[0-9]+\\.[0-9]+) +(\\-?[0-9]+\\.[0-9]+)"));
            auto match = regex.match(content);
            if(match.hasMatch())
                holidays = match.captured(2);
            else
            {
                holidays = tr("n/a");
                qWarning() << "holidays not found in PDF";
            }
        }

        setText(tr("Balance: %0, Holidays: %1").arg(balance).arg(holidays));

        {
            QTemporaryFile file(QDir::temp().absoluteFilePath(QStringLiteral("reportXXXXXX.pdf")));
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
