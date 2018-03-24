QT += core network gui widgets uitools

TARGET = zeiterfassungguilib
TEMPLATE = lib

CONFIG += c++14

PROJECT_ROOT = ..

DESTDIR = $${OUT_PWD}/$${PROJECT_ROOT}/bin

LIBS += -L$$DESTDIR -lzeiterfassungcorelib

INCLUDEPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib
DEPENDPATH += $$PWD/$${PROJECT_ROOT}/zeiterfassungcorelib $$PWD/$${PROJECT_ROOT}/zeiterfassungguilib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT
DEFINES += ZEITERFASSUNGGUILIB_LIBRARY

SOURCES += mainwindow.cpp \
           settingswidget.cpp \
           stripfactory.cpp \
           stripswidget.cpp \
           zeiterfassungdialog.cpp \
           zeiterfassungplugin.cpp \
           dialogs/authenticationdialog.cpp \
           dialogs/languageselectiondialog.cpp \
           dialogs/settingsdialog.cpp

HEADERS += mainwindow.h \
           settingswidget.h \
           stripfactory.h \
           stripswidget.h \
           zeiterfassungguilib_global.h \
           zeiterfassungdialog.h \
           zeiterfassungplugin.h \
           dialogs/authenticationdialog.h \
           dialogs/languageselectiondialog.h \
           dialogs/settingsdialog.h

FORMS += mainwindow.ui \
         dialogs/settingsdialog.ui \
         dialogs/languageselectiondialog.ui \
         dialogs/authenticationdialog.ui

RESOURCES += zeiterfassungguilib_resources.qrc

TRANSLATIONS +=  translations/zeiterfassungguilib_en.ts \
                 translations/zeiterfassungguilib_de.ts

include($${PROJECT_ROOT}/ccache.pri)

include($${PROJECT_ROOT}/lrelease.pri)

include(QStringListWidget/QStringListWidget.pri)
