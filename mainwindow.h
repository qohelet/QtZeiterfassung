#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "zeiterfassung.h"

class QSettings;
class QLabel;

namespace Ui { class MainWindow; }
class BuchungenModel;
class KontierungenModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QSettings &settings, Zeiterfassung &erfassung, const Zeiterfassung::UserInfo &userInfo, QWidget *parent = 0);
    ~MainWindow();

    static int timeToSeconds(const QTime &time);
    static QTime timeBetween(const QTime &l, const QTime &r);
    static QTime timeAdd(const QTime &l, const QTime &r);
    static QTime timeNormalise(const QTime &time);

private Q_SLOTS:
    void refresh();
    void getProjekteFinished(bool success, const QString &message, const QVector<Zeiterfassung::Projekt> &projekte);
    void getAuswertungFinished(bool success, const QString &message, const QByteArray &content);
    void refreshBuchungenFinished(bool success, const QString &message);
    void refreshKontierungenFinished(bool success, const QString &message);
    void contextMenuBuchung(const QPoint &pos);
    void contextMenuKontierung(const QPoint &pos);
    void pushButtonPrevPressed();
    void pushButtonNextPressed();
    void pushButtonStartPressed();
    void pushButtonEndPressed();

private:
    void validateEntries();
    void updateComboboxes();
    void updateAuswertung();
    void clearStrips();

    Ui::MainWindow *ui;
    QSettings &m_settings;
    Zeiterfassung &m_erfassung;
    const Zeiterfassung::UserInfo &m_userInfo;
    QVector<Zeiterfassung::Projekt> m_projekte;
    QByteArray m_auswertung;
    QLabel *m_auswertungLabel;

    BuchungenModel *m_buchungenModel;
    KontierungenModel *m_kontierungenModel;

    bool m_flag;
    QTime m_kontierungTime;
    QTime m_lastKontierungStart;
};

#endif // MAINWINDOW_H
