#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

#include "zeiterfassung.h"

class QLabel;

namespace Ui { class MainWindow; }
class ZeiterfassungSettings;
class BuchungenModel;
class TimeAssignmentsModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ZeiterfassungSettings &settings, Zeiterfassung &erfassung, const Zeiterfassung::UserInfo &userInfo, QWidget *parent = 0);
    ~MainWindow();

    static int timeToSeconds(const QTime &time);
    static QTime timeBetween(const QTime &l, const QTime &r);
    static QTime timeAdd(const QTime &l, const QTime &r);
    static QTime timeNormalise(const QTime &time);

private Q_SLOTS:
    void refresh(bool forceAuswertung = false);
    void getProjekctsFinished(bool success, const QString &message, const QVector<Zeiterfassung::Project> &projects);
    void getAuswertungFinished(bool success, const QString &message, const QByteArray &content);
    void refreshBuchungenFinished(bool success, const QString &message);
    void refreshTimeAssignmentsFinished(bool success, const QString &message);
    void contextMenuBuchung(const QPoint &pos);
    void contextMenuTimeAssignment(const QPoint &pos);
    void pushButtonStartPressed();
    void pushButtonEndPressed();

private:
    void validateEntries();
    void updateComboboxes();
    void clearStrips();

    QString buildProjectString(const QString &project);

    Ui::MainWindow *ui;
    ZeiterfassungSettings &m_settings;
    Zeiterfassung &m_erfassung;
    const Zeiterfassung::UserInfo &m_userInfo;
    QMap<QString, QString> m_projects;
    QDate m_auswertungDate;
    QByteArray m_auswertung;
    QLabel *m_workingTimeLabel;
    QLabel *m_balanceLabel;
    QLabel *m_holidaysLabel;

    BuchungenModel *m_buchungenModel;
    TimeAssignmentsModel *m_timeAssignmentsModel;

    bool m_flag;
    QTime m_timeAssignmentTime;
    QTime m_lastTimeAssignmentStart;
};

#endif // MAINWINDOW_H
