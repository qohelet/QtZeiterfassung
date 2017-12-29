#pragma once

#include "zeiterfassungguilib_global.h"
#include "zeiterfassungdialog.h"

namespace Ui { class AuthenticationDialog; }

class ZEITERFASSUNGGUILIBSHARED_EXPORT AuthenticationDialog : public ZeiterfassungDialog
{
    Q_OBJECT

public:
    explicit AuthenticationDialog(QWidget *parent = Q_NULLPTR);
    ~AuthenticationDialog();

    QString username() const;
    void setUsername(const QString &username);

    QString password() const;
    void setPassword(const QString &password);

private:
    Ui::AuthenticationDialog *ui;
};
