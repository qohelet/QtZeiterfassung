#pragma once

#include <memory>

#include <QDialog>
#include <QNetworkReply>

class StripsWidget;

namespace Ui { class LunchMealDialog; }

class LunchMealDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LunchMealDialog(StripsWidget &stripsWidget);
    ~LunchMealDialog();

private Q_SLOTS:
    void finished();

private:
    Ui::LunchMealDialog *ui;
    std::unique_ptr<QNetworkReply> m_reply;
};
