QT += core network gui widgets

TARGET = zeiterfassung
TEMPLATE = app

CONFIG += c++14
CONFIG -= app_bundle

PROJECT_ROOT = ..

DESTDIR = $${OUT_PWD}/$${PROJECT_ROOT}/bin

LIBS += -L$${OUT_PWD}/$${PROJECT_ROOT}/bin -lzeiterfassungcorelib -lzeiterfassungguilib

INCLUDEPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib
DEPENDPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT

RC_ICONS = icon.ico

SOURCES += main.cpp

HEADERS +=

FORMS += strips/bookingstartstrip.ui \
         strips/bookingendstrip.ui \
         strips/timeassignmentstrip.ui

RESOURCES += zeiterfassung_resources.qrc

TRANSLATIONS += translations/zeiterfassung_en.ts \
                translations/zeiterfassung_de.ts

include($${PROJECT_ROOT}/lrelease.pri)

include(installs.pri)
unix: include(installs_unix.pri)
win32: include(installs_win32.pri)
