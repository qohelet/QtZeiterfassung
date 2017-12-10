#ifndef AUTHENTICATIONDIALOG_H
#define AUTHENTICATIONDIALOG_H

#include <QDialog>

namespace Ui {
class AuthenticationDialog;
}

class AuthenticationDialog : public QDialog
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

#endif // AUTHENTICATIONDIALOG_H
