#pragma once

#include <QTime>

#include "zeiterfassungdialog.h"

namespace Ui { class BookingDialog; }

class BookingDialog : public ZeiterfassungDialog
{
    Q_OBJECT

public:
    explicit BookingDialog(QWidget *parent = Q_NULLPTR);
    ~BookingDialog();

    QTime getTime() const;
    void setTime(const QTime &time);

    QTime getTimespan() const;
    void setTimespan(const QTime &timespan);

    QString getType() const;
    void setType(const QString &type);

    QString getText() const;
    void setText(const QString &text);

private:
    Ui::BookingDialog *ui;
};
