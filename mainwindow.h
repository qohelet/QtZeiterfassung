#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

#include "zeiterfassungapi.h"

class QLabel;
class QBoxLayout;

namespace Ui { class MainWindow; }
class ZeiterfassungSettings;
class GetProjectsReply;
class GetAuswertungReply;
class StripFactory;
class StripsWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ZeiterfassungSettings &settings, ZeiterfassungApi &erfassung, const ZeiterfassungApi::UserInfo &userInfo,
                        StripFactory &stripFactory, QWidget *parent = Q_NULLPTR);
    ~MainWindow();

private Q_SLOTS:
    void getProjectsFinished();
    void getAuswertungFinished();
    void contextMenuBooking(const QPoint &pos);
    void contextMenuTimeAssignment(const QPoint &pos);
    void pushButtonStartPressed();
    void pushButtonEndPressed();
    void dateChanged(bool force = false);
    void openAuswertung();

    void timeAssignmentTimeChanged();
    void minimumTimeChanged();
    void refreshingChanged();
    void startEnabledChanged();
    void endEnabledChanged();

private:
    void updateComboboxes();

    Ui::MainWindow *ui;
    ZeiterfassungSettings &m_settings;
    ZeiterfassungApi &m_erfassung;
    const ZeiterfassungApi::UserInfo &m_userInfo;

    GetProjectsReply *m_getProjectsReply;
    GetAuswertungReply *m_getAuswertungReply;

    QMap<QString, QString> m_projects;

    QDate m_auswertungDate;
    QByteArray m_auswertung;

    QLabel *m_workingTimeLabel;
    QLabel *m_balanceLabel;
    QLabel *m_holidaysLabel;

    StripFactory &m_stripFactory;
    std::array<QLabel*, 7> m_stripsWidgetHeaders;
    std::array<StripsWidget*, 7> m_stripsWidgets;
    StripsWidget *m_currentStripWidget;
};

#endif // MAINWINDOW_H
