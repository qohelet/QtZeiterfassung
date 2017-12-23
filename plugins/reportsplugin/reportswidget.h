#pragma once

#include <QLabel>
#include <QDate>
#include <QUrl>

#include "replies/getreportreply.h"

class QAction;

class MainWindow;

class ReportsWidget : public QLabel
{
    Q_OBJECT

public:
    explicit ReportsWidget(MainWindow &mainWindow);

private Q_SLOTS:
    void dateChanged(const QDate &date);
    void refresh();
    void finished();
    void openReport();

private:
    MainWindow &m_mainWindow;

    QAction *m_actionOpenReport;
    QAction *m_actionRefreshReport;

    QDate m_date;
    QUrl m_url;

    std::unique_ptr<GetReportReply> m_reply;
};
