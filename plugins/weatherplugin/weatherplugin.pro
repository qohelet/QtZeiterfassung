QT += core network gui widgets

TARGET = weatherplugin
TEMPLATE = lib

CONFIG += shared c++14

DESTDIR = $${OUT_PWD}/../../bin/plugins/zeiterfassung

LIBS += -L$$OUT_PWD/../../lib -lzeiterfassungguilib

INCLUDEPATH += $$PWD/../../zeiterfassungguilib
DEPENDPATH += $$PWD/../../zeiterfassungguilib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT

HEADERS += weatherplugin.h \
           weatherwidget.h

SOURCES += weatherplugin.cpp \
           weatherwidget.cpp

FORMS +=

RESOURCES +=

TRANSLATIONS += translations/weatherplugin_en.ts \
                translations/weatherplugin_de.ts

OTHER_FILES += weatherplugin.json

include(../../lrelease.pri)

COMPILED_TRANSLATIONS += $${OUT_PWD}/translations/weatherplugin_en.qm \
                         $${OUT_PWD}/translations/weatherplugin_de.qm

include(../copy_translations.pri)
