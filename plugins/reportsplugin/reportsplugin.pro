QT += core network gui widgets

TARGET = reportsplugin
TEMPLATE = lib

CONFIG += shared c++14

PROJECT_ROOT = ../..

DESTDIR = $${OUT_PWD}/$${PROJECT_ROOT}/bin/plugins/zeiterfassung

LIBS += -L$${OUT_PWD}/$${PROJECT_ROOT}/bin -lzeiterfassungcorelib -lzeiterfassungguilib

INCLUDEPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib
DEPENDPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib

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

include($${PROJECT_ROOT}/lrelease.pri)
