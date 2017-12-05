QT += network gui widgets

CONFIG += c++11
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RC_ICONS = icon.ico

SOURCES += main.cpp \
    zeiterfassung.cpp \
    mainwindow.cpp \
    dialogs/aboutmedialog.cpp \
    dialogs/authenticationdialog.cpp \
    dialogs/buchungdialog.cpp \
    models/buchungenmodel.cpp \
    dialogs/kontierungdialog.cpp \
    models/kontierungenmodel.cpp \
    eventloopwithstatus.cpp \
    strips/buchungstrip.cpp \
    strips/kontierungstrip.cpp \
    zeiterfassungsettings.cpp \
    dialogs/settingsdialog.cpp \
    dialogs/languageselectiondialog.cpp

HEADERS += \
    zeiterfassung.h \
    mainwindow.h \
    dialogs/aboutmedialog.h \
    dialogs/authenticationdialog.h \
    dialogs/buchungdialog.h \
    models/buchungenmodel.h \
    dialogs/kontierungdialog.h \
    models/kontierungenmodel.h \
    eventloopwithstatus.h \
    strips/buchungstrip.h \
    strips/kontierungstrip.h \
    zeiterfassungsettings.h \
    dialogs/settingsdialog.h \
    dialogs/languageselectiondialog.h

RESOURCES += \
    resources.qrc

FORMS += \
    mainwindow.ui \
    dialogs/aboutmedialog.ui \
    dialogs/authenticationdialog.ui \
    dialogs/buchungdialog.ui \
    dialogs/kontierungdialog.ui \
    strips/buchungstrip.ui \
    strips/kontierungstrip.ui \
    dialogs/settingsdialog.ui \
    dialogs/languageselectiondialog.ui
