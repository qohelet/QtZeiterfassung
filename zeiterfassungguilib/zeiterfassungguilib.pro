QT += core network gui widgets uitools

TARGET = zeiterfassungguilib
TEMPLATE = lib

CONFIG += c++14

DESTDIR = $${OUT_PWD}/../lib

LIBS += -L$$OUT_PWD/../lib -lzeiterfassungcorelib

INCLUDEPATH += $$PWD/../zeiterfassungcorelib
DEPENDPATH += $$PWD/../zeiterfassungcorelib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT
DEFINES += ZEITERFASSUNGGUILIB_LIBRARY

SOURCES += mainwindow.cpp \
           stripfactory.cpp \
           stripswidget.cpp \
           zeiterfassungplugin.cpp \
           dialogs/authenticationdialog.cpp \
           dialogs/languageselectiondialog.cpp \
           dialogs/settingsdialog.cpp

HEADERS += mainwindow.h \
           stripfactory.h \
           stripswidget.h \
           zeiterfassungguilib_global.h \
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

include(../lrelease.pri)

# unix {
#    target.path = /usr/lib
#    INSTALLS += target
# }
