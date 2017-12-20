QT += core network gui widgets

TARGET = updaterplugin
TEMPLATE = lib

CONFIG += shared c++14

DESTDIR = $${OUT_PWD}/../../bin/plugins/zeiterfassung

LIBS += -L$$OUT_PWD/../../lib -lzeiterfassungcorelib -lzeiterfassungguilib

INCLUDEPATH += $$PWD/../../zeiterfassungcorelib $$PWD/../../zeiterfassungguilib
DEPENDPATH += $$PWD/../../zeiterfassungcorelib $$PWD/../../zeiterfassungguilib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT

HEADERS += updaterdialog.h \
           updaterplugin.h

SOURCES += updaterdialog.cpp \
           updaterplugin.cpp

FORMS += updaterdialog.ui

RESOURCES +=

TRANSLATIONS += translations/updaterplugin_en.ts \
                translations/updaterplugin_de.ts

OTHER_FILES += updaterplugin.json

include(../../lrelease.pri)

COMPILED_TRANSLATIONS += $${OUT_PWD}/translations/updaterplugin_en.qm \
                         $${OUT_PWD}/translations/updaterplugin_de.qm

include(../copy_translations.pri)
