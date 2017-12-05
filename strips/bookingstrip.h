#ifndef BOOKINGSTRIP_H
#define BOOKINGSTRIP_H

#include <QFrame>

class QTime;

class ZeiterfassungSettings;
namespace Ui { class BookingStrip; }

class BookingStrip : public QFrame
{
    Q_OBJECT

public:
    explicit BookingStrip(int id, const QTime &time, const QString &type, const ZeiterfassungSettings &settings, QWidget *parent = 0);
    ~BookingStrip();

private:
    Ui::BookingStrip *ui;
};

#endif // BOOKINGSTRIP_H
