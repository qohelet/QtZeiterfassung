#pragma once

#include <QDialog>
#include <QUrl>

class QNetworkReply;

namespace Ui { class UpdaterDialog; }
class MainWindow;

class UpdaterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdaterDialog(MainWindow &parent);
    ~UpdaterDialog();

private Q_SLOTS:
    void acceptedSlot();
    void rejectedSlot();
    void finished();

private:
    Ui::UpdaterDialog *ui;
    MainWindow &m_mainWindow;
    QNetworkReply *m_reply;

    QUrl m_url;
};
