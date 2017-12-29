#pragma once

#include "zeiterfassungdialog.h"
#include "replies/getuserinforeply.h"

namespace Ui { class ProfileDialog; }

class ProfileDialog : public ZeiterfassungDialog
{
    Q_OBJECT

public:
    explicit ProfileDialog(const GetUserInfoReply::UserInfo &userInfo, QWidget *parent = Q_NULLPTR);
    ~ProfileDialog();

private:
    Ui::ProfileDialog *ui;
};
