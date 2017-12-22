#include "webradiodialog.h"
#include "ui_webradiodialog.h"

#include <QMediaPlayer>

WebRadioDialog::WebRadioDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WebRadioDialog),
    m_player(new QMediaPlayer(this))
{
    ui->setupUi(this);

    for(const auto &url : QStringList {
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
    })
    {
        ui->comboBox->addItem(url);
    }

    connect(ui->comboBox, &QComboBox::currentTextChanged, this, [=](const QString &url){ m_player->setMedia(QMediaContent(QUrl(url))); });

    Q_EMIT ui->comboBox->currentTextChanged(ui->comboBox->currentText());

    connect(ui->pushButtonPlay, &QAbstractButton::pressed, m_player, &QMediaPlayer::play);
    connect(ui->pushButtonPause, &QAbstractButton::pressed, m_player, &QMediaPlayer::pause);
    connect(ui->pushButtonStop, &QAbstractButton::pressed, m_player, &QMediaPlayer::stop);
    connect(ui->horizontalSlider, &QAbstractSlider::valueChanged, m_player, &QMediaPlayer::setVolume);

    connect(m_player, &QMediaPlayer::stateChanged, [](QMediaPlayer::State newState){ qDebug() << newState; });
    connect(m_player, &QMediaPlayer::mediaStatusChanged, [](QMediaPlayer::MediaStatus status){ qDebug() << status; });
    connect(m_player, static_cast<void(QMediaPlayer::*)(QMediaPlayer::Error)>(&QMediaPlayer::error),
            [](QMediaPlayer::Error error){ qDebug() << error; });
    connect(m_player, SIGNAL(volumeChanged(int)), ui->horizontalSlider, SLOT(setValue(int)));

    Q_EMIT m_player->volumeChanged(m_player->volume());
}

WebRadioDialog::~WebRadioDialog()
{
    delete ui;
}

void WebRadioDialog::play()
{

}
