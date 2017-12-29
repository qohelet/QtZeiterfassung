#pragma once

#include <memory>

#include <QToolButton>
#include <QNetworkReply>

class StripsWidget;

class LunchMealWidget : public QToolButton
{
    Q_OBJECT
public:
    explicit LunchMealWidget(StripsWidget &stripsWidget);

private Q_SLOTS:
    void pressedSlot();
    void dateChanged(const QDate &date);
    void finished();

private:
    StripsWidget &m_stripsWidget;
    std::unique_ptr<QNetworkReply> m_reply;
    QString m_content;
};
