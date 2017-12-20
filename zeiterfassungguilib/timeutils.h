#pragma once

#include <QTime>

#include "zeiterfassungguilib_global.h"

int ZEITERFASSUNGGUILIBSHARED_EXPORT timeToSeconds(const QTime &time);
QTime ZEITERFASSUNGGUILIBSHARED_EXPORT timeBetween(const QTime &l, const QTime &r);
QTime ZEITERFASSUNGGUILIBSHARED_EXPORT timeAdd(const QTime &l, const QTime &r);
QTime ZEITERFASSUNGGUILIBSHARED_EXPORT timeNormalise(const QTime &time);
