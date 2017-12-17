QT += core network gui widgets

TARGET = zeiterfassung
TEMPLATE = app

CONFIG += c++14
CONFIG -= app_bundle

DESTDIR = $${OUT_PWD}/../bin

LIBS += -L$$OUT_PWD/../lib -lzeiterfassunglib

INCLUDEPATH += $$PWD/../zeiterfassunglib
DEPENDPATH += $$PWD/../zeiterfassunglib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT

RC_ICONS = icon.ico

SOURCES += main.cpp

HEADERS +=

FORMS += strips/bookingstartstrip.ui \
         strips/bookingendstrip.ui \
         strips/timeassignmentstrip.ui

RESOURCES +=

TRANSLATIONS += translations/zeiterfassung_en.ts \
                translations/zeiterfassung_de.ts

include(../lrelease.pri)

include(installs.pri)
unix: include(installs_unix.pri)
win32: include(installs_win32.pri)
