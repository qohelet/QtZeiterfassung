#pragma once

#include <QDialog>

#include "zeiterfassungguilib_global.h"
#include "replies/getuserinforeply.h"

namespace Ui { class AboutMeDialog; }

class ZEITERFASSUNGGUILIBSHARED_EXPORT AboutMeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutMeDialog(const GetUserInfoReply::UserInfo &userInfo, QWidget *parent = Q_NULLPTR);
    ~AboutMeDialog();

private:
    Ui::AboutMeDialog *ui;
};
