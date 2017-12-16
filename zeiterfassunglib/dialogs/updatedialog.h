#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QDialog>
#include <QUrl>

#include "zeiterfassunglib_global.h"

class QNetworkAccessManager;
class QNetworkReply;

class ZeiterfassungSettings;
namespace Ui { class UpdateDialog; }

class ZEITERFASSUNGLIBSHARED_EXPORT UpdateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateDialog(ZeiterfassungSettings &settings, QNetworkAccessManager *manager, QWidget *parent = Q_NULLPTR);
    ~UpdateDialog();

private Q_SLOTS:
    void finished();
    void submit();

private:
    Ui::UpdateDialog *ui;
    ZeiterfassungSettings &m_settings;
    QNetworkReply *m_reply;

    QUrl m_url;
};

#endif // UPDATEDIALOG_H
