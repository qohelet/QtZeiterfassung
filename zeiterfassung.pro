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
    eventloopwithstatus.cpp \
    zeiterfassungsettings.cpp \
    dialogs/settingsdialog.cpp \
    dialogs/languageselectiondialog.cpp \
    dialogs/timeassignmentdialog.cpp \
    models/timeassignmentsmodel.cpp \
    strips/timeassignmentstrip.cpp \
    dialogs/bookingdialog.cpp \
    models/bookingsmodel.cpp \
    strips/bookingstrip.cpp \
    dialogs/updatedialog.cpp

HEADERS += \
    zeiterfassung.h \
    mainwindow.h \
    dialogs/aboutmedialog.h \
    dialogs/authenticationdialog.h \
    eventloopwithstatus.h \
    zeiterfassungsettings.h \
    dialogs/settingsdialog.h \
    dialogs/languageselectiondialog.h \
    dialogs/timeassignmentdialog.h \
    models/timeassignmentsmodel.h \
    strips/timeassignmentstrip.h \
    dialogs/bookingdialog.h \
    models/bookingsmodel.h \
    strips/bookingstrip.h \
    dialogs/updatedialog.h

FORMS += \
    mainwindow.ui \
    dialogs/aboutmedialog.ui \
    dialogs/authenticationdialog.ui \
    dialogs/settingsdialog.ui \
    dialogs/languageselectiondialog.ui \
    dialogs/timeassignmentdialog.ui \
    strips/timeassignmentstrip.ui \
    dialogs/bookingdialog.ui \
    strips/bookingstrip.ui \
    dialogs/updatedialog.ui

RESOURCES += \
    resources.qrc

TRANSLATIONS += \
    translations/zeiterfassung_en.ts \
    translations/zeiterfassung_de.ts

win32 {
    CONFIG(debug, release|debug) {
        translationsinstall.path = $${OUT_PWD}/debug/translations
    } else {
        translationsinstall.path = $${OUT_PWD}/release/translations
    }
}
unix {
    translationsinstall.path = $${OUT_PWD}/translations
}

translationsinstall.files = $$[QT_INSTALL_TRANSLATIONS]/qt_en.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qtbase_en.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qtmultimedia_en.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qtquick1_en.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qtscript_nen.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_en.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qt_de.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qtbase_de.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qtmultimedia_de.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qtquick1_de.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qtscript_de.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_de.qm
INSTALLS *= translationsinstall
