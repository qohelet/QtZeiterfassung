#ifndef WEBRADIODIALOG_H
#define WEBRADIODIALOG_H

#include <QDialog>
#include <QMediaPlayer>

class MainWindow;

namespace Ui { class WebRadioDialog; }

class WebRadioDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WebRadioDialog(MainWindow &mainWindow);
    ~WebRadioDialog();

private Q_SLOTS:
    void stateChanged(QMediaPlayer::State newState);
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);
    void error(QMediaPlayer::Error error);

    void volumeChanged(int volume);
    void play();

private:
    Ui::WebRadioDialog *ui;

    MainWindow &m_mainWindow;
    QMediaPlayer *m_player;
};

#endif // WEBRADIODIALOG_H
