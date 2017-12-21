QT += core network gui widgets

TARGET = profileplugin
TEMPLATE = lib

CONFIG += shared c++14

DESTDIR = $${OUT_PWD}/../../bin/plugins/zeiterfassung

LIBS += -L$$OUT_PWD/../../lib -lzeiterfassungcorelib -lzeiterfassungguilib

INCLUDEPATH += $$PWD/../../zeiterfassungcorelib $$PWD/../../zeiterfassungguilib
DEPENDPATH += $$PWD/../../zeiterfassungcorelib $$PWD/../../zeiterfassungguilib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT

HEADERS += profileplugin.h

SOURCES += profileplugin.cpp

FORMS +=

RESOURCES += profileplugin_resources.qrc

TRANSLATIONS += translations/profileplugin_en.ts \
                translations/profileplugin_de.ts

OTHER_FILES += profileplugin.json

include(../../lrelease.pri)

COMPILED_TRANSLATIONS += $${OUT_PWD}/translations/profileplugin_en.qm \
                         $${OUT_PWD}/translations/profileplugin_de.qm

include(../copy_translations.pri)
