QT += core network gui widgets

TARGET = weatherplugin
TEMPLATE = lib

CONFIG += shared c++14

PROJECT_ROOT = ../..

DESTDIR = $${OUT_PWD}/$${PROJECT_ROOT}/bin/plugins/zeiterfassung

LIBS += -L$${OUT_PWD}/$${PROJECT_ROOT}/bin -lzeiterfassungcorelib -lzeiterfassungguilib

INCLUDEPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib
DEPENDPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib

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
