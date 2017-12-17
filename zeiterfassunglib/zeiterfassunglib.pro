QT += core network gui widgets uitools

TARGET = zeiterfassunglib
TEMPLATE = lib

CONFIG += c++14

DESTDIR = $${OUT_PWD}/../lib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT
DEFINES += ZEITERFASSUNGLIB_LIBRARY

SOURCES += mainwindow.cpp \
           stripfactory.cpp \
           stripswidget.cpp \
           timeutils.cpp \
           zeiterfassungapi.cpp \
           zeiterfassungplugin.cpp \
           zeiterfassungsettings.cpp \
           dialogs/aboutmedialog.cpp \
           dialogs/authenticationdialog.cpp \
           dialogs/bookingdialog.cpp \
           dialogs/languageselectiondialog.cpp \
           dialogs/settingsdialog.cpp \
           dialogs/timeassignmentdialog.cpp \
           dialogs/updatedialog.cpp \
           models/bookingsmodel.cpp \
           models/timeassignmentsmodel.cpp \
           replies/createbookingreply.cpp \
           replies/createtimeassignmentreply.cpp \
           replies/deletebookingreply.cpp \
           replies/deletetimeassignmentreply.cpp \
           replies/getauswertungreply.cpp \
           replies/getbookingsreply.cpp \
           replies/getpresencestatusreply.cpp \
           replies/getprojectsreply.cpp \
           replies/gettimeassignmentsreply.cpp \
           replies/loginpagereply.cpp \
           replies/loginreply.cpp \
           replies/updatebookingreply.cpp \
           replies/updatetimeassignmentreply.cpp \
           replies/zeiterfassungreply.cpp \
    replies/getuserinforeply.cpp

HEADERS += cpp14polyfills.h \
           mainwindow.h \
           stripfactory.h \
           stripswidget.h \
           timeutils.h \
           zeiterfassungapi.h \
           zeiterfassunglib_global.h \
           zeiterfassungplugin.h \
           zeiterfassungsettings.h \
           dialogs/aboutmedialog.h \
           dialogs/authenticationdialog.h \
           dialogs/bookingdialog.h \
           dialogs/languageselectiondialog.h \
           dialogs/settingsdialog.h \
           dialogs/timeassignmentdialog.h \
           dialogs/updatedialog.h \
           models/bookingsmodel.h \
           models/timeassignmentsmodel.h \
           replies/createbookingreply.h \
           replies/createtimeassignmentreply.h \
           replies/deletebookingreply.h \
           replies/deletetimeassignmentreply.h \
           replies/getauswertungreply.h \
           replies/getbookingsreply.h \
           replies/getpresencestatusreply.h \
           replies/getprojectsreply.h \
           replies/gettimeassignmentsreply.h \
           replies/loginpagereply.h \
           replies/loginreply.h \
           replies/updatebookingreply.h \
           replies/updatetimeassignmentreply.h \
           replies/zeiterfassungreply.h \
    replies/getuserinforeply.h

FORMS += mainwindow.ui \
         dialogs/updatedialog.ui \
         dialogs/settingsdialog.ui \
         dialogs/languageselectiondialog.ui \
         dialogs/authenticationdialog.ui \
         dialogs/bookingdialog.ui \
         dialogs/aboutmedialog.ui \
         dialogs/timeassignmentdialog.ui

RESOURCES += resources.qrc

TRANSLATIONS +=  translations/zeiterfassunglib_en.ts \
                 translations/zeiterfassunglib_de.ts

include(../lrelease.pri)

# unix {
#    target.path = /usr/lib
#    INSTALLS += target
# }
