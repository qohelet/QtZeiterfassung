#ifndef KONTIERUNGSTRIP_H
#define KONTIERUNGSTRIP_H

#include <QFrame>

class QTime;

namespace Ui { class KontierungStrip; }

class KontierungStrip : public QFrame
{
    Q_OBJECT

public:
    explicit KontierungStrip(const QTime &startTime, const QTime &endTime, const QString &projekt,
                             const QString &subprojekt, const QString &workpackage, const QString &text,
                             QWidget *parent = 0);
    ~KontierungStrip();

private:
    Ui::KontierungStrip *ui;
};

#endif // KONTIERUNGSTRIP_H
