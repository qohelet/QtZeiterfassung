QT += core network gui widgets

TARGET = presenceplugin
TEMPLATE = lib

CONFIG += shared c++14

DESTDIR = $${OUT_PWD}/../../bin/plugins/zeiterfassung

LIBS += -L$$OUT_PWD/../../lib -lzeiterfassunglib

INCLUDEPATH += $$PWD/../../zeiterfassunglib
DEPENDPATH += $$PWD/../../zeiterfassunglib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT

HEADERS += presenceplugin.h \
           presencewidget.h

SOURCES += presenceplugin.cpp \
           presencewidget.cpp

FORMS +=

RESOURCES += presenceplugin_resources.qrc

TRANSLATIONS +=

OTHER_FILES += presenceplugin.json
