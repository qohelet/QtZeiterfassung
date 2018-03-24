QT += core network gui widgets multimedia

TARGET = webradioplugin
TEMPLATE = lib

CONFIG += shared c++14

PROJECT_ROOT = ../..

DESTDIR = $${OUT_PWD}/$${PROJECT_ROOT}/bin/plugins/zeiterfassung

LIBS += -L$${OUT_PWD}/$${PROJECT_ROOT}/bin -lzeiterfassungcorelib -lzeiterfassungguilib

INCLUDEPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib
DEPENDPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT

HEADERS += webradiodialog.h \
           webradioplugin.h \
           webradiosettings.h \
           webradiosettingswidget.h

SOURCES += webradiodialog.cpp \
           webradioplugin.cpp \
           webradiosettings.cpp \
           webradiosettingswidget.cpp

FORMS += webradiodialog.ui

RESOURCES += webradioplugin_resources.qrc

TRANSLATIONS += translations/webradioplugin_en.ts \
                translations/webradioplugin_de.ts

OTHER_FILES += webradioplugin.json

include($${PROJECT_ROOT}/ccache.pri)

include($${PROJECT_ROOT}/lrelease.pri)
