#ifndef TIMEUTILS_H
#define TIMEUTILS_H

#include <QTime>

int timeToSeconds(const QTime &time);
QTime timeBetween(const QTime &l, const QTime &r);
QTime timeAdd(const QTime &l, const QTime &r);
QTime timeNormalise(const QTime &time);

#endif // TIMEUTILS_H
