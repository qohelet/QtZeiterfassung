#include "webradiodialog.h"
#include "ui_webradiodialog.h"

#include <QMediaPlayer>

WebRadioDialog::WebRadioDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WebRadioDialog),
    m_player(new QMediaPlayer(this))
{
    ui->setupUi(this);

    m_player->setMedia(QMediaContent(QUrl(QStringLiteral("http://stream.drumandbass.fm:9002"))));

    connect(ui->pushButton, &QAbstractButton::pressed, m_player, &QMediaPlayer::play);
}

WebRadioDialog::~WebRadioDialog()
{
    delete ui;
}

void WebRadioDialog::play()
{

}
