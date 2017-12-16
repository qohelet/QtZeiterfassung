#ifndef ABOUTMEDIALOG_H
#define ABOUTMEDIALOG_H

#include <QDialog>

#include "zeiterfassunglib_global.h"
#include "zeiterfassungapi.h"

namespace Ui { class AboutMeDialog; }

class ZEITERFASSUNGLIBSHARED_EXPORT AboutMeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutMeDialog(const ZeiterfassungApi::UserInfo &userInfo, QWidget *parent = Q_NULLPTR);
    ~AboutMeDialog();

private:
    Ui::AboutMeDialog *ui;
    const ZeiterfassungApi::UserInfo &m_userInfo;
};

#endif // ABOUTMEDIALOG_H
