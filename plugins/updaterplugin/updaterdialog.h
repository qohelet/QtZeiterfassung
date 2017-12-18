#ifndef UPDATERDIALOG_H
#define UPDATERDIALOG_H

#include <QDialog>
#include <QUrl>

#include "zeiterfassunglib_global.h"

class QNetworkReply;

namespace Ui { class UpdaterDialog; }
class MainWindow;

class ZEITERFASSUNGLIBSHARED_EXPORT UpdaterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdaterDialog(MainWindow &parent);
    ~UpdaterDialog();

private Q_SLOTS:
    void finished();

private:
    Ui::UpdaterDialog *ui;
    MainWindow &m_mainWindow;
    QNetworkReply *m_reply;

    QUrl m_url;
};

#endif // UPDATERDIALOG_H
