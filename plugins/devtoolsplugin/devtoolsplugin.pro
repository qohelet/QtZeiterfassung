QT += core network gui widgets

TARGET = devtoolsplugin
TEMPLATE = lib

CONFIG += shared c++14

PROJECT_ROOT = ../..

DESTDIR = $${OUT_PWD}/$${PROJECT_ROOT}/bin/plugins/zeiterfassung

LIBS += -L$${OUT_PWD}/$${PROJECT_ROOT}/bin -lzeiterfassungcorelib -lzeiterfassungguilib

INCLUDEPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib
DEPENDPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT

HEADERS += devtoolsplugin.h \
           logdialog.h \
           logmodel.h

SOURCES += devtoolsplugin.cpp \
           logdialog.cpp \
           logmodel.cpp

FORMS += logdialog.ui

RESOURCES += devtoolsplugin_resources.qrc

TRANSLATIONS += translations/devtoolsplugin_en.ts \
                translations/devtoolsplugin_de.ts

OTHER_FILES += devtoolsplugin.json

include($${PROJECT_ROOT}/ccache.pri)

include($${PROJECT_ROOT}/lrelease.pri)
