#pragma once

#include <QMediaPlayer>

#include "zeiterfassungdialog.h"

#include "webradiosettings.h"

class MainWindow;

namespace Ui { class WebRadioDialog; }

class WebRadioDialog : public ZeiterfassungDialog
{
    Q_OBJECT

public:
    explicit WebRadioDialog(MainWindow &mainWindow);
    ~WebRadioDialog();

private Q_SLOTS:
    void stateChanged(QMediaPlayer::State newState);
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);
    void error(QMediaPlayer::Error error);
    void currentIndexChanged(int index);
    void volumeChanged(int volume);
    void play();

private:
    void updateWidgets();

    Ui::WebRadioDialog *ui;

    MainWindow &m_mainWindow;
    WebRadioSettings m_settings;

    QMediaPlayer *m_player;
};
