QT += network gui widgets uitools

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
    stripfactory.cpp \
    zeiterfassungapi.cpp \
    replies/loginpagereply.cpp \
    replies/loginreply.cpp \
    replies/userinforeply.cpp \
    replies/getbookingsreply.cpp \
    replies/createbookingreply.cpp \
    replies/updatebookingreply.cpp \
    replies/deletebookingreply.cpp \
    replies/gettimeassignmentsreply.cpp \
    replies/createtimeassignmentreply.cpp \
    replies/updatetimeassignmentreply.cpp \
    replies/getprojectsreply.cpp \
    replies/getauswertungreply.cpp \
    replies/zeiterfassungreply.cpp \
    replies/deletetimeassignmentreply.cpp

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
    stripfactory.h \
    zeiterfassungapi.h \
    replies/loginpagereply.h \
    replies/loginreply.h \
    replies/userinforeply.h \
    replies/getbookingsreply.h \
    replies/createbookingreply.h \
    replies/updatebookingreply.h \
    replies/deletebookingreply.h \
    replies/gettimeassignmentsreply.h \
    replies/createtimeassignmentreply.h \
    replies/updatetimeassignmentreply.h \
    replies/getprojectsreply.h \
    replies/getauswertungreply.h \
    replies/zeiterfassungreply.h \
    replies/deletetimeassignmentreply.h

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

win32 {
    CONFIG(debug, release|debug) {
        translationsinstall.path = $${OUT_PWD}/debug/translations
        themesinstall.path = $${OUT_PWD}/debug/themes
        darkthemeinstall.path = $${OUT_PWD}/debug/themes/dark_theme
        stripsinstall.path = $${OUT_PWD}/debug/strips
    } else {
        translationsinstall.path = $${OUT_PWD}/release/translations
        themesinstall.path = $${OUT_PWD}/release/themes
        darkthemeinstall.path = $${OUT_PWD}/release/themes/dark_theme
        stripsinstall.path = $${OUT_PWD}/release/strips
    }
}
unix {
    translationsinstall.path = $${OUT_PWD}/translations
    themesinstall.path = $${OUT_PWD}/themes
    darkthemeinstall.path = $${OUT_PWD}/themes/dark_theme
    stripsinstall.path = $${OUT_PWD}/strips
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
                            $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_de.qm \
                            translations/zeiterfassung_en.qm \
                            translations/zeiterfassung_de.qm

themesinstall.files = themes/dark_theme.qss

darkthemeinstall.files = themes/dark_theme/checkbox_indeterminate_disabled.png \
                         themes/dark_theme/radio_unchecked.png \
                         themes/dark_theme/up_arrow.png \
                         themes/dark_theme/branch_closed-on.png \
                         themes/dark_theme/checkbox_checked_disabled.png \
                         themes/dark_theme/checkbox_unchecked.png \
                         themes/dark_theme/checkbox_indeterminate.png \
                         themes/dark_theme/stylesheet-branch-more.png \
                         themes/dark_theme/checkbox_checked.png \
                         themes/dark_theme/checkbox_unchecked_disabled.png \
                         themes/dark_theme/radio_checked.png \
                         themes/dark_theme/checkbox_indeterminate_focus.png \
                         themes/dark_theme/checkbox_checked_focus.png \
                         themes/dark_theme/branch_closed.png \
                         themes/dark_theme/Vsepartoolbar.png \
                         themes/dark_theme/radio_checked_disabled.png \
                         themes/dark_theme/left_arrow.png \
                         themes/dark_theme/Vmovetoolbar.png \
                         themes/dark_theme/branch_open-on.png \
                         themes/dark_theme/close.png \
                         themes/dark_theme/stylesheet-branch-end.png \
                         themes/dark_theme/stylesheet-vline.png \
                         themes/dark_theme/down_arrow_disabled.png \
                         themes/dark_theme/radio_unchecked_disabled.png \
                         themes/dark_theme/left_arrow_disabled.png \
                         themes/dark_theme/Hmovetoolbar.png \
                         themes/dark_theme/close-pressed.png \
                         themes/dark_theme/up_arrow_disabled.png \
                         themes/dark_theme/branch_open.png \
                         themes/dark_theme/radio_checked_focus.png \
                         themes/dark_theme/sizegrip.png \
                         themes/dark_theme/checkbox_unchecked_focus.png \
                         themes/dark_theme/right_arrow_disabled.png \
                         themes/dark_theme/Hsepartoolbar.png \
                         themes/dark_theme/undock.png \
                         themes/dark_theme/transparent.png \
                         themes/dark_theme/close-hover.png \
                         themes/dark_theme/radio_unchecked_focus.png \
                         themes/dark_theme/down_arrow.png \
                         themes/dark_theme/right_arrow.png

stripsinstall.files = strips/bookingstartstrip.ui \
                      strips/bookingendstrip.ui \
                      strips/timeassignmentstrip.ui

INSTALLS += translationsinstall
INSTALLS += themesinstall
INSTALLS += darkthemeinstall
INSTALLS += stripsinstall
