#pragma once

#include <QTime>

#include "zeiterfassunglib_global.h"

int ZEITERFASSUNGLIBSHARED_EXPORT timeToSeconds(const QTime &time);
QTime ZEITERFASSUNGLIBSHARED_EXPORT timeBetween(const QTime &l, const QTime &r);
QTime ZEITERFASSUNGLIBSHARED_EXPORT timeAdd(const QTime &l, const QTime &r);
QTime ZEITERFASSUNGLIBSHARED_EXPORT timeNormalise(const QTime &time);
