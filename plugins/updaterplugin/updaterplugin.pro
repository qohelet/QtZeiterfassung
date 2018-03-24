QT += core network gui widgets

TARGET = updaterplugin
TEMPLATE = lib

CONFIG += shared c++14

PROJECT_ROOT = ../..

DESTDIR = $${OUT_PWD}/$${PROJECT_ROOT}/bin/plugins/zeiterfassung

LIBS += -L$${OUT_PWD}/$${PROJECT_ROOT}/bin -lzeiterfassungcorelib -lzeiterfassungguilib

INCLUDEPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib
DEPENDPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT

HEADERS += updaterdialog.h \
           updaterplugin.h \
    updatersettings.h \
    updatersettingswidget.h

SOURCES += updaterdialog.cpp \
           updaterplugin.cpp \
    updatersettings.cpp \
    updatersettingswidget.cpp

FORMS += updaterdialog.ui

RESOURCES +=

TRANSLATIONS += translations/updaterplugin_en.ts \
                translations/updaterplugin_de.ts

OTHER_FILES += updaterplugin.json

include(../../lrelease.pri)
