#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QMap>

#include "zeiterfassunglib_global.h"
#include "replies/getuserinforeply.h"
#include "replies/getprojectsreply.h"
#include "replies/getpresencestatusreply.h"

class QMenu;
class QToolBar;
class QLabel;
class QBoxLayout;

namespace Ui { class MainWindow; }
class ZeiterfassungSettings;
class StripFactory;
class StripsWidget;

class ZEITERFASSUNGLIBSHARED_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ZeiterfassungSettings &settings, ZeiterfassungApi &erfassung, const GetUserInfoReply::UserInfo &userInfo,
                        StripFactory &stripFactory, QWidget *parent = Q_NULLPTR);
    ~MainWindow();

    QMenu *menuFile() const;
    QMenu *menuView() const;
    QMenu *menuTools() const;
    QMenu *menuAbout() const;
    QToolBar *toolBar() const;

    ZeiterfassungSettings &settings() const;
    ZeiterfassungApi &erfassung() const;
    const GetUserInfoReply::UserInfo &userInfo() const;
    StripFactory &stripFactory() const;

    QDate date() const;

    const QMap<QString, QString> &projects() const;
    const std::array<StripsWidget*, 7> &stripsWidgets() const;

private Q_SLOTS:
    void getProjectsFinished();
    void pushButtonStartPressed();
    void pushButtonEndPressed();
    void dateChanged(bool force = false);

    void minimumTimeChanged();
    void refreshingChanged();
    void startEnabledChanged();
    void endEnabledChanged();

private:
    void updateComboboxes();

    Ui::MainWindow *ui;
    ZeiterfassungSettings &m_settings;
    ZeiterfassungApi &m_erfassung;
    const GetUserInfoReply::UserInfo &m_userInfo;
    StripFactory &m_stripFactory;

    std::unique_ptr<GetProjectsReply> m_getProjectsReply;

    QMap<QString, QString> m_projects;

    std::array<StripsWidget*, 7> m_stripsWidgets;
    StripsWidget *m_currentStripWidget;
};

#endif // MAINWINDOW_H
