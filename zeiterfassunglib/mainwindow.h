#pragma once

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
    void setDate(const QDate &date);

    const QMap<QString, QString> &projects() const;
    const std::array<StripsWidget*, 7> &stripsWidgets() const;

Q_SIGNALS:
    void dateChanged(const QDate &date);
    void refreshEverything();

private Q_SLOTS:
    void getProjectsFinished();
    void pushButtonStartPressed();
    void pushButtonEndPressed();
    void dateChangedSlot(const QDate &date);

    void minimumTimeChanged();
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
