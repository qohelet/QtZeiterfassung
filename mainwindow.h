#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

#include "zeiterfassungapi.h"

class QLabel;
class QBoxLayout;

namespace Ui { class MainWindow; }
class ZeiterfassungSettings;
class BookingsModel;
class TimeAssignmentsModel;
class StripFactory;
class StripsWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ZeiterfassungSettings &settings, ZeiterfassungApi &erfassung, const ZeiterfassungApi::UserInfo &userInfo, QWidget *parent = 0);
    ~MainWindow();

private Q_SLOTS:
    void refresh(bool forceAuswertung = false);
    void getProjekctsFinished(bool success, const QString &message, const QVector<ZeiterfassungApi::Project> &projects);
    void getAuswertungFinished(bool success, const QString &message, const QByteArray &content);
    void refreshBookingsFinished(bool success, const QString &message);
    void refreshTimeAssignmentsFinished(bool success, const QString &message);
    void contextMenuBooking(const QPoint &pos);
    void contextMenuTimeAssignment(const QPoint &pos);
    void pushButtonStartPressed();
    void pushButtonEndPressed();

private:
    void updateComboboxes();
    void createStrips();

    Ui::MainWindow *ui;
    ZeiterfassungSettings &m_settings;
    ZeiterfassungApi &m_erfassung;
    const ZeiterfassungApi::UserInfo &m_userInfo;
    QMap<QString, QString> m_projects;

    QDate m_auswertungDate;
    QByteArray m_auswertung;

    QLabel *m_workingTimeLabel;
    QLabel *m_balanceLabel;
    QLabel *m_holidaysLabel;

    bool m_flag;
    BookingsModel *m_bookingsModel;
    TimeAssignmentsModel *m_timeAssignmentsModel;

    StripFactory *m_stripFactory;
    StripsWidget *m_stripsWidgets[7];
};

#endif // MAINWINDOW_H
