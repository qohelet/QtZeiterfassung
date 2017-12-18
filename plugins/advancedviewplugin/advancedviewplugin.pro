QT += core network gui widgets

TARGET = advancedviewplugin
TEMPLATE = lib

CONFIG += shared c++14

DESTDIR = $${OUT_PWD}/../../bin/plugins/zeiterfassung

LIBS += -L$$OUT_PWD/../../lib -lzeiterfassunglib

INCLUDEPATH += $$PWD/../../zeiterfassunglib
DEPENDPATH += $$PWD/../../zeiterfassunglib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT

HEADERS += advancedviewplugin.h \
           advancedviewwidget.h

SOURCES += advancedviewplugin.cpp \
           advancedviewwidget.cpp

FORMS +=

RESOURCES += \
    advancedviewplugin_resources.qrc

TRANSLATIONS +=

OTHER_FILES += advancedviewplugin.json
