#pragma once

#include <QTime>

#include "zeiterfassungcorelib_global.h"

int ZEITERFASSUNGCORELIBSHARED_EXPORT timeToSeconds(const QTime &time);
QTime ZEITERFASSUNGCORELIBSHARED_EXPORT timeBetween(const QTime &l, const QTime &r);
QTime ZEITERFASSUNGCORELIBSHARED_EXPORT timeAdd(const QTime &l, const QTime &r);
QTime ZEITERFASSUNGCORELIBSHARED_EXPORT timeNormalise(const QTime &time);
