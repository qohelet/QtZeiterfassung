#include "lunchmealdialog.h"
#include "ui_lunchmealdialog.h"

#include <QDate>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>

#include "stripswidget.h"
#include "mainwindow.h"
#include "zeiterfassungsettings.h"
#include "zeiterfassungapi.h"

LunchMealDialog::LunchMealDialog(StripsWidget &stripsWidget) :
    QDialog(&stripsWidget.mainWindow()),
    ui(new Ui::LunchMealDialog)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    ui->labelTitle->setText(tr("Lunch meal for %0").arg(stripsWidget.date().toString(tr("dd.MM.yyyy"))));

    const auto &settings = stripsWidget.mainWindow().settings();

    auto url = settings.value(QStringLiteral("LunchMealPlugin/url"),
                              QStringLiteral("https://brunner.ninja/lunch/%0.txt")).toString()
            .arg(stripsWidget.date().toString(settings.value(QStringLiteral("LunchMealPlugin/dateFormat"),
                                                             QStringLiteral("yyyy-MM-dd")).toString()));
    m_reply = std::unique_ptr<QNetworkReply>(stripsWidget.mainWindow().erfassung().manager()->get(QNetworkRequest(QUrl(url))));
    connect(m_reply.get(), &QNetworkReply::finished, this, &LunchMealDialog::finished);
}

LunchMealDialog::~LunchMealDialog()
{
    delete ui;
}

void LunchMealDialog::finished()
{
    if(m_reply->error() != QNetworkReply::NoError)
    {
        ui->labelLunchMeal->setText(QStringLiteral("<span style=\"color: red;\">%0</span>\n\n%1")
                                    .arg(m_reply->errorString())
                                    .arg(QString(m_reply->readAll())));
        goto after;
    }

    ui->labelLunchMeal->setText(m_reply->readAll());

    after:
    m_reply = Q_NULLPTR;
}
