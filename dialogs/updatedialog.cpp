#include "updatedialog.h"
#include "ui_updatedialog.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>

UpdateDialog::UpdateDialog(ZeiterfassungSettings &settings, QNetworkAccessManager *manager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateDialog),
    m_settings(settings)
{
    ui->setupUi(this);

    m_reply = manager->get(QNetworkRequest(QUrl(QStringLiteral("https://api.github.com/repos/0xFEEDC0DE64/QtZeiterfassung/releases"))));
    connect(m_reply, &QNetworkReply::finished, this, &UpdateDialog::finished);
}

UpdateDialog::~UpdateDialog()
{
    delete ui;
}

void UpdateDialog::finished()
{
    if(m_reply->error() != QNetworkReply::NoError)
    {
        qWarning() << m_reply->error() << m_reply->errorString();
        return;
    }

    qDebug() << m_reply->readAll();
}
