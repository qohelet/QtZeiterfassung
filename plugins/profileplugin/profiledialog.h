#pragma once

#include <QDialog>

#include "zeiterfassungguilib_global.h"
#include "replies/getuserinforeply.h"

namespace Ui { class ProfileDialog; }

class ZEITERFASSUNGGUILIBSHARED_EXPORT ProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProfileDialog(const GetUserInfoReply::UserInfo &userInfo, QWidget *parent = Q_NULLPTR);
    ~ProfileDialog();

private:
    Ui::ProfileDialog *ui;
};
