#ifndef REPORTSWIDGET_H
#define REPORTSWIDGET_H

#include <QWidget>
#include <QDate>
#include <QUrl>

#include "replies/getauswertungreply.h"

class QLabel;
class QAction;

class MainWindow;

class ReportsWidget : public QWidget
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

    QLabel *m_labelBalance;
    QLabel *m_labelHolidays;

    QAction *m_actionOpenReport;
    QAction *m_actionRefreshReport;

    QDate m_date;
    QUrl m_url;

    std::unique_ptr<GetAuswertungReply> m_reply;
};

#endif // REPORTSWIDGET_H
