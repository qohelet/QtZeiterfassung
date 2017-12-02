#ifndef BUCHUNGDIALOG_H
#define BUCHUNGDIALOG_H

#include <QDialog>

namespace Ui {
class BuchungDialog;
}

class BuchungDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BuchungDialog(QWidget *parent = 0);
    ~BuchungDialog();

    QTime getTime() const;
    void setTime(const QTime &time);

    QTime getTimespan() const;
    void setTimespan(const QTime &timespan);

    QString getType() const;
    void setType(const QString &type);

    QString getText() const;
    void setText(const QString &text);

private:
    Ui::BuchungDialog *ui;
};

#endif // BUCHUNGDIALOG_H
