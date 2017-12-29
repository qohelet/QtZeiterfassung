#include "weatherwidget.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include "mainwindow.h"
#include "zeiterfassungsettings.h"
#include "zeiterfassungapi.h"

WeatherWidget::WeatherWidget(MainWindow &mainWindow) :
    QLabel(&mainWindow),
    m_mainWindow(mainWindow)
{
    setFrameShape(QFrame::Panel);
    setFrameShadow(QFrame::Sunken);

    connect(&m_mainWindow, &MainWindow::refreshEverything, this, &WeatherWidget::refresh);

    refresh();
}

void WeatherWidget::refresh()
{
    setText(tr("Loading..."));

    auto url = m_mainWindow.settings().value(QStringLiteral("WeatherPlugin/url"),
    QStringLiteral("http://api.openweathermap.org/data/2.5/weather?q=Graz,AT&units=metric&APPID=40f6c892c6162680c6c9235169dc9f83")).toString();

    m_reply = std::unique_ptr<QNetworkReply>(m_mainWindow.erfassung().manager()->get(QNetworkRequest(QUrl(url))));
    connect(m_reply.get(), &QNetworkReply::finished, this, &WeatherWidget::finished);
}

void WeatherWidget::finished()
{
    if(m_reply->error() != QNetworkReply::NoError)
    {
        qWarning() << m_reply->errorString();
        setText(tr("Request failed"));
        goto after;
    }

    {
        QJsonParseError error;

        auto document = QJsonDocument::fromJson(m_reply->readAll(), &error);
        if(error.error != QJsonParseError::NoError)
        {
            qWarning() << error.errorString();
            setText(tr("Parsing failed"));
            goto after;
        }

        if(!document.isObject())
        {
            setText(tr("Not an json obj"));
            goto after;
        }

        auto obj = document.object();

        if(!obj.contains(QStringLiteral("weather")))
        {
            qWarning() << "no weather" << obj;
            setText(tr("No weather found"));
            goto after;
        }

        if(!obj.contains(QStringLiteral("main")))
        {
            qWarning() << "no main" << obj;
            setText(tr("No main found"));
            goto after;
        }

        auto weaterObj = obj.value(QStringLiteral("weather")).toArray();
        auto mainObj = obj.value(QStringLiteral("main")).toObject();

        setText(tr("%0 (%1°C)").arg(weaterObj.first().toObject().value(QStringLiteral("main")).toString())
                .arg(mainObj.value(QStringLiteral("temp")).toDouble()));
    }

    after:
    m_reply = Q_NULLPTR;
}
