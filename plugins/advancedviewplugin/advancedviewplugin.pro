QT += core network gui widgets

TARGET = advancedviewplugin
TEMPLATE = lib

CONFIG += shared c++14

PROJECT_ROOT = ../..

DESTDIR = $${OUT_PWD}/$${PROJECT_ROOT}/bin/plugins/zeiterfassung

LIBS += -L$${OUT_PWD}/$${PROJECT_ROOT}/bin -lzeiterfassungcorelib -lzeiterfassungguilib

INCLUDEPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib
DEPENDPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT

HEADERS += advancedviewdialog.h \
           advancedviewplugin.h \
           advancedviewwidget.h \
           dialogs/bookingdialog.h \
           dialogs/timeassignmentdialog.h \
           models/bookingsmodel.h \
           models/timeassignmentsmodel.h

SOURCES += advancedviewdialog.cpp \
           advancedviewplugin.cpp \
           advancedviewwidget.cpp \
           dialogs/bookingdialog.cpp \
           dialogs/timeassignmentdialog.cpp \
           models/bookingsmodel.cpp \
           models/timeassignmentsmodel.cpp

FORMS += advancedviewdialog.ui \
         dialogs/bookingdialog.ui \
         dialogs/timeassignmentdialog.ui

RESOURCES += advancedviewplugin_resources.qrc

TRANSLATIONS += translations/advancedviewplugin_en.ts \
                translations/advancedviewplugin_de.ts

OTHER_FILES += advancedviewplugin.json

include($${PROJECT_ROOT}/lrelease.pri)
