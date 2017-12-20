QT += core network gui widgets

TARGET = presenceplugin
TEMPLATE = lib

CONFIG += shared c++14

DESTDIR = $${OUT_PWD}/../../bin/plugins/zeiterfassung

LIBS += -L$$OUT_PWD/../../lib -lzeiterfassungguilib

INCLUDEPATH += $$PWD/../../zeiterfassungguilib
DEPENDPATH += $$PWD/../../zeiterfassungguilib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT

HEADERS += presenceplugin.h \
           presencewidget.h

SOURCES += presenceplugin.cpp \
           presencewidget.cpp

FORMS +=

RESOURCES += presenceplugin_resources.qrc

TRANSLATIONS += translations/presenceplugin_en.ts \
                translations/presenceplugin_de.ts

OTHER_FILES += presenceplugin.json

include(../../lrelease.pri)

COMPILED_TRANSLATIONS += $${OUT_PWD}/translations/presenceplugin_en.qm \
                         $${OUT_PWD}/translations/presenceplugin_de.qm

include(../copy_translations.pri)
