QT += core network gui widgets

TARGET = presenceplugin
TEMPLATE = lib

CONFIG += shared c++14

PROJECT_ROOT = ../..

DESTDIR = $${OUT_PWD}/$${PROJECT_ROOT}/bin/plugins/zeiterfassung

LIBS += -L$${OUT_PWD}/$${PROJECT_ROOT}/bin -lzeiterfassungcorelib -lzeiterfassungguilib

INCLUDEPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib
DEPENDPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib

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
