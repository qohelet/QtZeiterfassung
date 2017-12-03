#ifndef KONTIERUNGDIALOG_H
#define KONTIERUNGDIALOG_H

#include <QDialog>
#include <QTime>

template <class Key, class T> class QMap;

class ZeiterfassungSettings;

namespace Ui { class KontierungDialog; }

class KontierungDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KontierungDialog(const QMap<QString, QString> &projekte, const ZeiterfassungSettings &settings,
                              QWidget *parent = 0);
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
};

#endif // KONTIERUNGDIALOG_H
