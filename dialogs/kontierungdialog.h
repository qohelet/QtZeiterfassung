#ifndef KONTIERUNGDIALOG_H
#define KONTIERUNGDIALOG_H

#include <QDialog>

#include "zeiterfassung.h"

namespace Ui { class KontierungDialog; }

class KontierungDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KontierungDialog(Zeiterfassung &erfassung, const Zeiterfassung::UserInfo &userInfo,
                              const QMap<QString, QString> &projekte, QWidget *parent = 0);
    ~KontierungDialog();

    QTime getTime() const;
    void setTime(const QTime &time);

    QTime getTimespan() const;
    void setTimespan(const QTime &timespan);

    QString getProjekt() const;
    void setProjekt(const QString &projekt);

    QString getSubprojekt() const;
    void setSubprojekt(const QString &subprojekt);

    QString getWorkpackage() const;
    void setWorkpackage(const QString &workpackage);

    QString getText() const;
    void setText(const QString &text);

private:
    Ui::KontierungDialog *ui;
    Zeiterfassung &m_erfassung;
    const Zeiterfassung::UserInfo &m_userInfo;
};

#endif // KONTIERUNGDIALOG_H
