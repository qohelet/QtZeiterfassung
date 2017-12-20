QT += core network gui widgets

TARGET = reportsplugin
TEMPLATE = lib

CONFIG += shared c++14

DESTDIR = $${OUT_PWD}/../../bin/plugins/zeiterfassung

LIBS += -L$$OUT_PWD/../../lib -lzeiterfassungcorelib -lzeiterfassungguilib

INCLUDEPATH += $$PWD/../../zeiterfassungcorelib $$PWD/../../zeiterfassungguilib
DEPENDPATH += $$PWD/../../zeiterfassungcorelib $$PWD/../../zeiterfassungguilib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT

HEADERS += reportsplugin.h \
    reportswidget.h

SOURCES += reportsplugin.cpp \
    reportswidget.cpp

FORMS +=

RESOURCES += reportsplugin_resources.qrc

TRANSLATIONS += translations/reportsplugin_en.ts \
                translations/reportsplugin_de.ts

OTHER_FILES += reportsplugin.json

include(../../lrelease.pri)

COMPILED_TRANSLATIONS += $${OUT_PWD}/translations/reportsplugin_en.qm \
                         $${OUT_PWD}/translations/reportsplugin_de.qm

include(../copy_translations.pri)
