#ifndef WEBRADIODIALOG_H
#define WEBRADIODIALOG_H

#include <QDialog>

class QMediaPlayer;

namespace Ui { class WebRadioDialog; }

class WebRadioDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WebRadioDialog(QWidget *parent = 0);
    ~WebRadioDialog();

private Q_SLOTS:
    void play();

private:
    Ui::WebRadioDialog *ui;

    QMediaPlayer *m_player;
};

#endif // WEBRADIODIALOG_H
