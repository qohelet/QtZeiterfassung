QT += core network gui widgets

TARGET = updaterplugin
TEMPLATE = lib

CONFIG += shared c++14

DESTDIR = $${OUT_PWD}/../../bin/plugins/zeiterfassung

LIBS += -L$$OUT_PWD/../../lib -lzeiterfassunglib

INCLUDEPATH += $$PWD/../../zeiterfassunglib
DEPENDPATH += $$PWD/../../zeiterfassunglib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT

HEADERS += updaterdialog.h \
           updaterplugin.h

SOURCES += updaterdialog.cpp \
           updaterplugin.cpp

FORMS += updaterdialog.ui

RESOURCES +=

TRANSLATIONS +=

OTHER_FILES += updaterplugin.json
