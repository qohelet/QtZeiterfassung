QT += core network gui widgets uitools

TARGET = zeiterfassungguilib
TEMPLATE = lib

CONFIG += c++14

DESTDIR = $${OUT_PWD}/../lib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT
DEFINES += ZEITERFASSUNGGUILIB_LIBRARY

SOURCES += mainwindow.cpp \
           stripfactory.cpp \
           stripswidget.cpp \
           timeutils.cpp \
           zeiterfassungapi.cpp \
           zeiterfassungplugin.cpp \
           zeiterfassungsettings.cpp \
           dialogs/aboutmedialog.cpp \
           dialogs/authenticationdialog.cpp \
           dialogs/languageselectiondialog.cpp \
           dialogs/settingsdialog.cpp \
           replies/createbookingreply.cpp \
           replies/createtimeassignmentreply.cpp \
           replies/deletebookingreply.cpp \
           replies/deletetimeassignmentreply.cpp \
           replies/getbookingsreply.cpp \
           replies/getpresencestatusreply.cpp \
           replies/getprojectsreply.cpp \
           replies/gettimeassignmentsreply.cpp \
           replies/loginpagereply.cpp \
           replies/loginreply.cpp \
           replies/updatebookingreply.cpp \
           replies/updatetimeassignmentreply.cpp \
           replies/zeiterfassungreply.cpp \
           replies/getuserinforeply.cpp \
           replies/getreportreply.cpp

HEADERS += cpp14polyfills.h \
           mainwindow.h \
           stripfactory.h \
           stripswidget.h \
           timeutils.h \
           zeiterfassungapi.h \
           zeiterfassungguilib_global.h \
           zeiterfassungplugin.h \
           zeiterfassungsettings.h \
           dialogs/aboutmedialog.h \
           dialogs/authenticationdialog.h \
           dialogs/languageselectiondialog.h \
           dialogs/settingsdialog.h \
           replies/createbookingreply.h \
           replies/createtimeassignmentreply.h \
           replies/deletebookingreply.h \
           replies/deletetimeassignmentreply.h \
           replies/getbookingsreply.h \
           replies/getpresencestatusreply.h \
           replies/getprojectsreply.h \
           replies/gettimeassignmentsreply.h \
           replies/loginpagereply.h \
           replies/loginreply.h \
           replies/updatebookingreply.h \
           replies/updatetimeassignmentreply.h \
           replies/zeiterfassungreply.h \
           replies/getuserinforeply.h \
           replies/getreportreply.h

FORMS += mainwindow.ui \
         dialogs/settingsdialog.ui \
         dialogs/languageselectiondialog.ui \
         dialogs/authenticationdialog.ui \
         dialogs/aboutmedialog.ui

RESOURCES += zeiterfassungguilib_resources.qrc

TRANSLATIONS +=  translations/zeiterfassungguilib_en.ts \
                 translations/zeiterfassungguilib_de.ts

include(../lrelease.pri)

# unix {
#    target.path = /usr/lib
#    INSTALLS += target
# }
