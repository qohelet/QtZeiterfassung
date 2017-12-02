#ifndef BUCHUNGSTRIP_H
#define BUCHUNGSTRIP_H

#include <QFrame>

class QTime;

namespace Ui { class BuchungStrip; }

class BuchungStrip : public QFrame
{
    Q_OBJECT

public:
    explicit BuchungStrip(int id, const QTime &time, const QString &type, QWidget *parent = 0);
    ~BuchungStrip();

private:
    Ui::BuchungStrip *ui;
};

#endif // BUCHUNGSTRIP_H
