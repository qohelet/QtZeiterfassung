QT += core network gui widgets

TARGET = lunchmealplugin
TEMPLATE = lib

CONFIG += shared c++14

DESTDIR = $${OUT_PWD}/../../bin/plugins/zeiterfassung

LIBS += -L$$OUT_PWD/../../lib -lzeiterfassungguilib

INCLUDEPATH += $$PWD/../../zeiterfassungguilib
DEPENDPATH += $$PWD/../../zeiterfassungguilib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT

HEADERS += lunchmealplugin.h \
           lunchmealwidget.h \
           lunchmealdialog.h

SOURCES += lunchmealplugin.cpp \
           lunchmealwidget.cpp \
           lunchmealdialog.cpp

FORMS += lunchmealdialog.ui

RESOURCES += lunchmealplugin_resources.qrc

TRANSLATIONS += translations/lunchmealplugin_en.ts \
                translations/lunchmealplugin_de.ts

OTHER_FILES += lunchmealplugin.json

include(../../lrelease.pri)

COMPILED_TRANSLATIONS += $${OUT_PWD}/translations/lunchmealplugin_en.qm \
                         $${OUT_PWD}/translations/lunchmealplugin_de.qm

include(../copy_translations.pri)
