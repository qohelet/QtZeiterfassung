QT += core network
QT -= gui widgets

TARGET = zeiterfassungcorelib
TEMPLATE = lib

CONFIG += c++14

PROJECT_ROOT = ..

DESTDIR = $${OUT_PWD}/$${PROJECT_ROOT}/bin

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT
DEFINES += ZEITERFASSUNGCORELIB_LIBRARY

SOURCES += timeutils.cpp \
           zeiterfassungapi.cpp \
           zeiterfassungsettings.cpp \
           replies/createbookingreply.cpp \
           replies/createtimeassignmentreply.cpp \
           replies/deletebookingreply.cpp \
           replies/deletetimeassignmentreply.cpp \
           replies/getabsencesreply.cpp \
           replies/getbookingsreply.cpp \
           replies/getpresencestatusreply.cpp \
           replies/getprojectsreply.cpp \
           replies/getreportreply.cpp \
           replies/gettimeassignmentsreply.cpp \
           replies/getuserinforeply.cpp \
           replies/loginpagereply.cpp \
           replies/loginreply.cpp \
           replies/updatebookingreply.cpp \
           replies/updatetimeassignmentreply.cpp \
           replies/zeiterfassungreply.cpp

HEADERS += cpp14polyfills.h \
           timeutils.h \
           zeiterfassungapi.h \
           zeiterfassungcorelib_global.h \
           zeiterfassungsettings.h \
           replies/createbookingreply.h \
           replies/createtimeassignmentreply.h \
           replies/deletebookingreply.h \
           replies/deletetimeassignmentreply.h \
           replies/getabsencesreply.h \
           replies/getbookingsreply.h \
           replies/getpresencestatusreply.h \
           replies/getprojectsreply.h \
           replies/getreportreply.h \
           replies/gettimeassignmentsreply.h \
           replies/getuserinforeply.h \
           replies/loginpagereply.h \
           replies/loginreply.h \
           replies/updatebookingreply.h \
           replies/updatetimeassignmentreply.h \
           replies/zeiterfassungreply.h

FORMS +=

RESOURCES +=

TRANSLATIONS +=  translations/zeiterfassungcorelib_en.ts \
                 translations/zeiterfassungcorelib_de.ts

include($${PROJECT_ROOT}ccache.pri)

include($${PROJECT_ROOT}/lrelease.pri)
