#include "updatedialog.h"
#include "ui_updatedialog.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QVersionNumber>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QDesktopServices>
#include <QMessageBox>

#include "zeiterfassungsettings.h"

UpdateDialog::UpdateDialog(ZeiterfassungSettings &settings, QNetworkAccessManager *manager, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateDialog),
    m_settings(settings)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &UpdateDialog::submit);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, [=](){
        if(ui->checkBoxDontShow->isChecked())
            m_settings.setLastUpdateCheck(QDate::currentDate());
        reject();
    });

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

    auto appVersion = QVersionNumber::fromString(QCoreApplication::applicationVersion());

    auto array = document.array();

    for(const auto &releaseVal : array)
    {
        auto releaseObj = releaseVal.toObject();
        auto version = QVersionNumber::fromString(releaseObj.value("tag_name").toString());

        if(appVersion < version)
        {
            m_url = QUrl(releaseObj.value("html_url").toString());
            ui->labelDescription->setText(releaseObj.value("body").toString());

            show();
            return;
        }
    }

    deleteLater();
}

void UpdateDialog::submit()
{
    if(ui->checkBoxDontShow->isChecked())
        m_settings.setLastUpdateCheck(QDate::currentDate());

    if(!QDesktopServices::openUrl(m_url))
        QMessageBox::warning(this, tr("Could not open default webbrowser!"), tr("Could not open default webbrowser!"));

    accept();
}
