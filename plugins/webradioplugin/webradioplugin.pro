QT += core network gui widgets multimedia

TARGET = webradioplugin
TEMPLATE = lib

CONFIG += shared c++14

DESTDIR = $${OUT_PWD}/../../bin/plugins/zeiterfassung

LIBS += -L$$OUT_PWD/../../lib -lzeiterfassungcorelib -lzeiterfassungguilib

INCLUDEPATH += $$PWD/../../zeiterfassungcorelib $$PWD/../../zeiterfassungguilib
DEPENDPATH += $$PWD/../../zeiterfassungcorelib $$PWD/../../zeiterfassungguilib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT

HEADERS += webradioplugin.h \
    webradiodialog.h

SOURCES += webradioplugin.cpp \
    webradiodialog.cpp

FORMS += \
    webradiodialog.ui

RESOURCES += webradioplugin_resources.qrc

TRANSLATIONS += translations/webradioplugin_en.ts \
                translations/webradioplugin_de.ts

OTHER_FILES += webradioplugin.json

include(../../lrelease.pri)

COMPILED_TRANSLATIONS += $${OUT_PWD}/translations/webradioplugin_en.qm \
                         $${OUT_PWD}/translations/webradioplugin_de.qm

include(../copy_translations.pri)
