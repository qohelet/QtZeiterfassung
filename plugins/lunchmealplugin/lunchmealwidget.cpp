#include "lunchmealwidget.h"

#include <QIcon>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QMessageBox>
#include <QStringBuilder>

#include "stripswidget.h"
#include "mainwindow.h"
#include "zeiterfassungsettings.h"
#include "zeiterfassungapi.h"

#include "lunchmealdialog.h"

LunchMealWidget::LunchMealWidget(StripsWidget &stripsWidget) :
    QToolButton(&stripsWidget),
    m_stripsWidget(stripsWidget)
{
    setIcon(QIcon(QStringLiteral(":/zeiterfassung/plugins/lunchmealplugin/images/lunch-meal.png")));
    setText(tr("Lunch meal"));

    connect(this, &QAbstractButton::pressed, this, &LunchMealWidget::pressedSlot);

    connect(&m_stripsWidget, &StripsWidget::dateChanged, this, &LunchMealWidget::dateChanged);
    dateChanged(m_stripsWidget.date());
}

void LunchMealWidget::pressedSlot()
{
    LunchMealDialog dialog(m_stripsWidget.date(), m_content, this);
    dialog.exec();
}

void LunchMealWidget::dateChanged(const QDate &date)
{
    qDebug() << "called";

    setEnabled(false);
    setVisible(false);

    const auto &settings = m_stripsWidget.mainWindow().settings();

    auto url = settings.value(QStringLiteral("LunchMealPlugin/url"),
                              QStringLiteral("https://brunner.ninja/lunch/%0.txt")).toString()
            .arg(date.toString(settings.value(QStringLiteral("LunchMealPlugin/dateFormat"), QStringLiteral("yyyy-MM-dd")).toString()));
    m_reply = std::unique_ptr<QNetworkReply>(m_stripsWidget.mainWindow().erfassung().manager()->get(QNetworkRequest(QUrl(url))));
    connect(m_reply.get(), &QNetworkReply::finished, this, &LunchMealWidget::finished);
}

void LunchMealWidget::finished()
{
    if(m_reply->error() != QNetworkReply::NoError &&
       m_reply->error() != QNetworkReply::ContentNotFoundError)
    {
        QMessageBox::warning(&m_stripsWidget.mainWindow(), tr("Could not load lunch meal!"),
                             tr("Could not load lunch meal!") % "\n\n" % m_reply->errorString());
        goto after;
    }

    if(m_reply->error() == QNetworkReply::NoError)
    {
        setEnabled(true);
        setVisible(true);
        m_content = m_reply->readAll();
    }

    after:
    m_reply = Q_NULLPTR;
}
