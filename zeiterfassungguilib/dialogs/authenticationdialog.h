#pragma once

#include <QDialog>

#include "zeiterfassungguilib_global.h"

namespace Ui {
class AuthenticationDialog;
}

class ZEITERFASSUNGGUILIBSHARED_EXPORT AuthenticationDialog : public QDialog
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
