#include "webradiodialog.h"
#include "ui_webradiodialog.h"

#include "mainwindow.h"
#include "zeiterfassungsettings.h"

WebRadioDialog::WebRadioDialog(MainWindow &mainWindow) :
    QDialog(&mainWindow),
    ui(new Ui::WebRadioDialog),
    m_mainWindow(mainWindow)
{
    ui->setupUi(this);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
#else
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
#endif

    m_player = new QMediaPlayer(this);
    connect(m_player, &QMediaPlayer::stateChanged, this, &WebRadioDialog::stateChanged);
    connect(m_player, &QMediaPlayer::stateChanged, this, &WebRadioDialog::updateWidgets);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &WebRadioDialog::mediaStatusChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &WebRadioDialog::updateWidgets);
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

    ui->comboBox->setCurrentIndex(ui->comboBox->findData(m_mainWindow.settings().value(QStringLiteral("WebRadioPlugin/lastUrl")).toString()));

    connect(ui->comboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &WebRadioDialog::currentIndexChanged);

    connect(ui->comboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &WebRadioDialog::updateWidgets);

    connect(ui->pushButtonPlay, &QAbstractButton::pressed, this, &WebRadioDialog::play);
    connect(ui->pushButtonPause, &QAbstractButton::pressed, m_player, &QMediaPlayer::pause);
    connect(ui->pushButtonStop, &QAbstractButton::pressed, m_player, &QMediaPlayer::stop);

    m_player->setVolume(m_mainWindow.settings().value(QStringLiteral("WebRadioPlugin/volume"), 100).toInt());
    ui->horizontalSlider->setValue(m_player->volume());
    connect(ui->horizontalSlider, &QAbstractSlider::valueChanged, this, &WebRadioDialog::volumeChanged);

    stateChanged(m_player->state());
    mediaStatusChanged(m_player->mediaStatus());
    currentIndexChanged(ui->comboBox->currentIndex());
    updateWidgets();
}

WebRadioDialog::~WebRadioDialog()
{
    // To avoid crash on app close
    disconnect(m_player, &QMediaPlayer::stateChanged, this, &WebRadioDialog::stateChanged);
    disconnect(m_player, &QMediaPlayer::stateChanged, this, &WebRadioDialog::updateWidgets);
    disconnect(m_player, &QMediaPlayer::mediaStatusChanged, this, &WebRadioDialog::mediaStatusChanged);
    disconnect(m_player, &QMediaPlayer::mediaStatusChanged, this, &WebRadioDialog::updateWidgets);
    disconnect(m_player, static_cast<void(QMediaPlayer::*)(QMediaPlayer::Error)>(&QMediaPlayer::error), this, &WebRadioDialog::error);

    delete ui;
}

void WebRadioDialog::stateChanged(QMediaPlayer::State newState)
{
    switch(newState)
    {
    case QMediaPlayer::StoppedState: ui->labelState->setText(tr("Stopped")); break;
    case QMediaPlayer::PlayingState: ui->labelState->setText(tr("Playing")); break;
    case QMediaPlayer::PausedState: ui->labelState->setText(tr("Paused")); break;
    default:
        qWarning() << "unknown state" << newState;
        ui->labelState->setText(tr("Unknown"));
    }
}

void WebRadioDialog::mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    switch(status)
    {
    case QMediaPlayer::UnknownMediaStatus: ui->labelMediaStatus->setText(tr("Unknown media")); break;
    case QMediaPlayer::NoMedia: ui->labelMediaStatus->setText(tr("No media")); break;
    case QMediaPlayer::LoadingMedia: ui->labelMediaStatus->setText(tr("Loading media...")); break;
    case QMediaPlayer::LoadedMedia: ui->labelMediaStatus->setText(tr("Loaded media")); break;
    case QMediaPlayer::StalledMedia: ui->labelMediaStatus->setText(tr("Stalled media")); break;
    case QMediaPlayer::BufferingMedia: ui->labelMediaStatus->setText(tr("Buffering media")); break;
    case QMediaPlayer::BufferedMedia: ui->labelMediaStatus->setText(tr("Buffered media")); break;
    case QMediaPlayer::EndOfMedia: ui->labelMediaStatus->setText(tr("End of media")); break;
    case QMediaPlayer::InvalidMedia: ui->labelMediaStatus->setText(tr("Invalid media")); break;
    default:
        qWarning() << "unknown mediaStatus" << status;
        ui->labelMediaStatus->setText(tr("Unknown"));
    }
}

void WebRadioDialog::error(QMediaPlayer::Error error)
{
    qWarning() << error;
}

void WebRadioDialog::currentIndexChanged(int index)
{
    if(index == -1)
        m_player->setMedia(QMediaContent());
    else
        m_player->setMedia(QMediaContent(QUrl(ui->comboBox->currentData().toString())));
}

void WebRadioDialog::play()
{
    if(ui->comboBox->currentIndex() == -1)
        return;

    m_mainWindow.settings().setValue(QStringLiteral("WebRadioPlugin/lastUrl"), ui->comboBox->currentData().toString());

    m_player->play();
}

void WebRadioDialog::updateWidgets()
{
    ui->comboBox->setEnabled(m_player->state() != QMediaPlayer::PlayingState);
    ui->pushButtonPlay->setEnabled(ui->comboBox->currentIndex() > -1 &&
                                   (m_player->state() == QMediaPlayer::StoppedState || m_player->state() == QMediaPlayer::PausedState) &&
                                   (m_player->mediaStatus() == QMediaPlayer::LoadedMedia || m_player->mediaStatus() == QMediaPlayer::BufferedMedia));
    ui->pushButtonPause->setEnabled(m_player->state() == QMediaPlayer::PlayingState || m_player->state() == QMediaPlayer::PausedState);
    ui->pushButtonStop->setEnabled(m_player->state() == QMediaPlayer::PlayingState || m_player->state() == QMediaPlayer::PausedState);
}

void WebRadioDialog::volumeChanged(int volume)
{
    m_mainWindow.settings().setValue(QStringLiteral("WebRadioPlugin/volume"), volume);
    m_player->setVolume(volume);
}
