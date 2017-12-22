#include "webradiodialog.h"
#include "ui_webradiodialog.h"

#include "mainwindow.h"
#include "zeiterfassungsettings.h"

WebRadioDialog::WebRadioDialog(MainWindow &mainWindow) :
    QDialog(&mainWindow),
    ui(new Ui::WebRadioDialog),
    m_mainWindow(mainWindow),
    m_player(new QMediaPlayer(this))
{
    ui->setupUi(this);

    connect(m_player, &QMediaPlayer::stateChanged, this, &WebRadioDialog::stateChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &WebRadioDialog::mediaStatusChanged);
    connect(m_player, static_cast<void(QMediaPlayer::*)(QMediaPlayer::Error)>(&QMediaPlayer::error), this, &WebRadioDialog::error);

    for(const auto &url : m_mainWindow.settings().value(QStringLiteral("WebRadioPlugin/urls"), QStringList {
        QStringLiteral("http://stream.drumandbass.fm:9002"),
        QStringLiteral("http://stream.trap.fm:6002"),
        QStringLiteral("http://stream.dubbase.fm:7002"),
        QStringLiteral("http://lw1.mp3.tb-group.fm/hb.mp3"),
        QStringLiteral("http://lw1.mp3.tb-group.fm/tb.mp3"),
        QStringLiteral("http://lw1.mp3.tb-group.fm/tt.mp3"),
        QStringLiteral("http://lw1.mp3.tb-group.fm/ht.mp3"),
        QStringLiteral("http://lw1.mp3.tb-group.fm/trb.mp3"),
        QStringLiteral("http://lw1.mp3.tb-group.fm/ct.mp3"),
        QStringLiteral("http://lw1.mp3.tb-group.fm/clt.mp3"),
        QStringLiteral("https://live.helsinki.at:8088/live160.ogg")
    }).toStringList())
    {
        ui->comboBox->addItem(url, url);
    }

    auto lastUrl = m_mainWindow.settings().value(QStringLiteral("WebRadioPlugin/lastUrl")).toString();
    qDebug() << lastUrl;
    auto index = ui->comboBox->findData(lastUrl);
    qDebug() << index;
    ui->comboBox->setCurrentIndex(index);

    connect(ui->pushButtonPlay, &QAbstractButton::pressed, this, &WebRadioDialog::play);
    connect(ui->pushButtonPause, &QAbstractButton::pressed, m_player, &QMediaPlayer::pause);
    connect(ui->pushButtonStop, &QAbstractButton::pressed, m_player, &QMediaPlayer::stop);

    m_player->setVolume(m_mainWindow.settings().value(QStringLiteral("WebRadioPlugin/volume"), 100).toInt());
    ui->horizontalSlider->setValue(m_player->volume());
    connect(ui->horizontalSlider, &QAbstractSlider::valueChanged, this, &WebRadioDialog::volumeChanged);
}

WebRadioDialog::~WebRadioDialog()
{
    delete ui;
}

void WebRadioDialog::stateChanged(QMediaPlayer::State newState)
{
    qDebug() << newState;
}

void WebRadioDialog::mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    qDebug() << status;
}

void WebRadioDialog::error(QMediaPlayer::Error error)
{
    qDebug() << error;
}

void WebRadioDialog::play()
{
    qDebug() << "called";
    if(ui->comboBox->currentIndex() == -1)
        return;

    m_mainWindow.settings().setValue(QStringLiteral("WebRadioPlugin/lastUrl"), ui->comboBox->currentData().toString());

    m_player->setMedia(QMediaContent(QUrl(ui->comboBox->currentData().toString())));
    m_player->play();
}

void WebRadioDialog::volumeChanged(int volume)
{
    m_mainWindow.settings().setValue(QStringLiteral("WebRadioPlugin/volume"), volume);
    m_player->setVolume(volume);
}
