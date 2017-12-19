#ifndef BOOKINGDIALOG_H
#define BOOKINGDIALOG_H

#include <QDialog>
#include <QTime>

namespace Ui { class BookingDialog; }

class BookingDialog : public QDialog
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

#endif // BOOKINGDIALOG_H
