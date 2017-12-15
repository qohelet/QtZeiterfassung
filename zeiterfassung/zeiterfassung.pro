QT += core network gui widgets uitools

TARGET = zeiterfassung
TEMPLATE = app

CONFIG += c++14
CONFIG -= app_bundle

DESTDIR = $${OUT_PWD}/../bin

LIBS += -L$$OUT_PWD/../lib -lzeiterfassunglib

INCLUDEPATH += $$PWD/../zeiterfassunglib
DEPENDPATH += $$PWD/../zeiterfassunglib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT

RC_ICONS = icon.ico

SOURCES += main.cpp \
    mainwindow.cpp \
    dialogs/aboutmedialog.cpp \
    dialogs/authenticationdialog.cpp \
    zeiterfassungsettings.cpp \
    dialogs/settingsdialog.cpp \
    dialogs/languageselectiondialog.cpp \
    dialogs/timeassignmentdialog.cpp \
    models/timeassignmentsmodel.cpp \
    dialogs/bookingdialog.cpp \
    models/bookingsmodel.cpp \
    dialogs/updatedialog.cpp \
    stripswidget.cpp \
    timeutils.cpp \
    stripfactory.cpp

HEADERS += \
    mainwindow.h \
    dialogs/aboutmedialog.h \
    dialogs/authenticationdialog.h \
    zeiterfassungsettings.h \
    dialogs/settingsdialog.h \
    dialogs/languageselectiondialog.h \
    dialogs/timeassignmentdialog.h \
    models/timeassignmentsmodel.h \
    dialogs/bookingdialog.h \
    models/bookingsmodel.h \
    dialogs/updatedialog.h \
    stripswidget.h \
    timeutils.h \
    stripfactory.h

FORMS += \
    mainwindow.ui \
    dialogs/aboutmedialog.ui \
    dialogs/authenticationdialog.ui \
    dialogs/settingsdialog.ui \
    dialogs/languageselectiondialog.ui \
    dialogs/timeassignmentdialog.ui \
    dialogs/bookingdialog.ui \
    dialogs/updatedialog.ui

RESOURCES += \
    resources.qrc

TRANSLATIONS += \
    translations/zeiterfassung_en.ts \
    translations/zeiterfassung_de.ts

include(installs.pri)
unix: include(installs_unix.pri)
win32: include(installs_win32.pri)
