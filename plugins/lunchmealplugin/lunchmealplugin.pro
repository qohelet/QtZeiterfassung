QT += core network gui widgets

TARGET = lunchmealplugin
TEMPLATE = lib

CONFIG += shared c++14

PROJECT_ROOT = ../..

DESTDIR = $${OUT_PWD}/$${PROJECT_ROOT}/bin/plugins/zeiterfassung

LIBS += -L$${OUT_PWD}/$${PROJECT_ROOT}/bin -lzeiterfassungcorelib -lzeiterfassungguilib

INCLUDEPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib
DEPENDPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT

HEADERS += lunchmealdialog.h \
           lunchmealsettings.h \
           lunchmealsettingswidget.h \
           lunchmealplugin.h \
           lunchmealwidget.h

SOURCES += lunchmealdialog.cpp \
           lunchmealsettings.cpp \
           lunchmealsettingswidget.cpp \
           lunchmealplugin.cpp \
           lunchmealwidget.cpp

FORMS += lunchmealdialog.ui

RESOURCES += lunchmealplugin_resources.qrc

TRANSLATIONS += translations/lunchmealplugin_en.ts \
                translations/lunchmealplugin_de.ts

OTHER_FILES += lunchmealplugin.json

include(../../lrelease.pri)
