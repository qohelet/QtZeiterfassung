#include "updatedialog.h"
#include "ui_updatedialog.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

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
        qWarning() << "request error" << m_reply->error() << m_reply->errorString();
        return;
    }

    QJsonParseError error;
    auto document = QJsonDocument::fromJson(m_reply->readAll(), &error);

    if(error.error != QJsonParseError::NoError)
    {
        qWarning() << "parse error" << error.error << error.errorString();
        return;
    }

    if(!document.isArray())
    {
        qWarning() << "document is not an array!";
        return;
    }

    auto array = document.array();

    for(const auto &releaseVal : array)
    {
        qDebug() << releaseVal;
    }
}
