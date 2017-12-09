#include "timeutils.h"

int timeToSeconds(const QTime &time)
{
    return QTime(0, 0).secsTo(time);
}

QTime timeBetween(const QTime &l, const QTime &r)
{
    Q_ASSERT(l <= r);
    return QTime(0, 0).addSecs(l.secsTo(r));
}

QTime timeAdd(const QTime &l, const QTime &r)
{
    Q_ASSERT(timeToSeconds(l) + timeToSeconds(r) < 86400);
    return l.addSecs(QTime(0, 0).secsTo(r));
}

QTime timeNormalise(const QTime &time)
{
    return QTime(time.hour(), time.minute());
}
