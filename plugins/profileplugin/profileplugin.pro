QT += core network gui widgets

TARGET = profileplugin
TEMPLATE = lib

CONFIG += shared c++14

PROJECT_ROOT = ../..

DESTDIR = $${OUT_PWD}/$${PROJECT_ROOT}/bin/plugins/zeiterfassung

LIBS += -L$${OUT_PWD}/$${PROJECT_ROOT}/bin -lzeiterfassungcorelib -lzeiterfassungguilib

INCLUDEPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib
DEPENDPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT

HEADERS += profiledialog.h \
           profileplugin.h

SOURCES += profiledialog.cpp \
           profileplugin.cpp

FORMS += profiledialog.ui

RESOURCES += profileplugin_resources.qrc

TRANSLATIONS += translations/profileplugin_en.ts \
                translations/profileplugin_de.ts

OTHER_FILES += profileplugin.json

include(../../lrelease.pri)
