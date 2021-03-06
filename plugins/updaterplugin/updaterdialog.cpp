#include "updaterdialog.h"
#include "ui_updaterdialog.h"

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

#include "mainwindow.h"
#include "zeiterfassungsettings.h"
#include "zeiterfassungapi.h"

#include "updatersettings.h"

UpdaterDialog::UpdaterDialog(MainWindow &mainWindow) :
    ZeiterfassungDialog(&mainWindow),
    ui(new Ui::UpdaterDialog),
    m_mainWindow(mainWindow)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &UpdaterDialog::acceptedSlot);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &UpdaterDialog::rejectedSlot);

    m_reply = m_mainWindow.erfassung().manager()->get(QNetworkRequest(UpdaterSettings(mainWindow.settings()).url()));
    connect(m_reply, &QNetworkReply::finished, this, &UpdaterDialog::finished);
}

UpdaterDialog::~UpdaterDialog()
{
    delete ui;
}

void UpdaterDialog::acceptedSlot()
{
    if(ui->checkBoxDontShow->isChecked())
        UpdaterSettings(m_mainWindow.settings()).setLastUpdateCheck(QDate::currentDate());

    if(!QDesktopServices::openUrl(m_url))
        QMessageBox::warning(this, tr("Could not open default webbrowser!"), tr("Could not open default webbrowser!"));

    accept();
}

void UpdaterDialog::rejectedSlot()
{
    if(ui->checkBoxDontShow->isChecked())
        UpdaterSettings(m_mainWindow.settings()).setLastUpdateCheck(QDate::currentDate());

    reject();
}

void UpdaterDialog::finished()
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
        auto version = QVersionNumber::fromString(releaseObj.value(QStringLiteral("tag_name")).toString());

        if(appVersion < version)
        {
            m_url = QUrl(releaseObj.value(QStringLiteral("html_url")).toString());
            ui->labelDescription->setText(releaseObj.value(QStringLiteral("body")).toString());

            show();
            return;
        }
    }

    UpdaterSettings(m_mainWindow.settings()).setLastUpdateCheck(QDate::currentDate());

    deleteLater();
}
