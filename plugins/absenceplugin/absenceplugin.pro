QT += core network gui widgets

TARGET = absenceplugin
TEMPLATE = lib

CONFIG += shared c++14

PROJECT_ROOT = ../..

DESTDIR = $${OUT_PWD}/$${PROJECT_ROOT}/bin/plugins/zeiterfassung

LIBS += -L$${OUT_PWD}/$${PROJECT_ROOT}/bin -lzeiterfassungcorelib -lzeiterfassungguilib

INCLUDEPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib
DEPENDPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT

HEADERS += absencedialog.h \
           absencesmodel.h \
           absenceplugin.h \
           absencewidget.h

SOURCES += absencedialog.cpp \
           absencesmodel.cpp \
           absenceplugin.cpp \
           absencewidget.cpp

FORMS += absencedialog.ui

RESOURCES += absenceplugin_resources.qrc

TRANSLATIONS += translations/absenceplugin_en.ts \
                translations/absenceplugin_de.ts

OTHER_FILES += absenceplugin.json

include(../../lrelease.pri)
