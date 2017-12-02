#ifndef ABOUTMEDIALOG_H
#define ABOUTMEDIALOG_H

#include <QDialog>

#include "zeiterfassung.h"

namespace Ui { class AboutMeDialog; }

class AboutMeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutMeDialog(const Zeiterfassung::UserInfo &userInfo, QWidget *parent = 0);
    ~AboutMeDialog();

private:
    Ui::AboutMeDialog *ui;
    const Zeiterfassung::UserInfo &m_userInfo;
};

#endif // ABOUTMEDIALOG_H
