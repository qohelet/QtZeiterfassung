QT += core network gui widgets

TARGET = devtoolsplugin
TEMPLATE = lib

CONFIG += shared c++14

DESTDIR = $${OUT_PWD}/../../bin/plugins/zeiterfassung

LIBS += -L$$OUT_PWD/../../lib -lzeiterfassungcorelib -lzeiterfassungguilib

INCLUDEPATH += $$PWD/../../zeiterfassungcorelib $$PWD/../../zeiterfassungguilib
DEPENDPATH += $$PWD/../../zeiterfassungcorelib $$PWD/../../zeiterfassungguilib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT

HEADERS += devtoolsplugin.h

SOURCES += devtoolsplugin.cpp

FORMS +=

RESOURCES +=

TRANSLATIONS += translations/devtoolsplugin_en.ts \
                translations/devtoolsplugin_de.ts

OTHER_FILES += devtoolsplugin.json

include(../../lrelease.pri)

COMPILED_TRANSLATIONS += $${OUT_PWD}/translations/devtoolsplugin_en.qm \
                         $${OUT_PWD}/translations/devtoolsplugin_de.qm

include(../copy_translations.pri)
