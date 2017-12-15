QT += core network gui widgets

TARGET = zeiterfassunglib
TEMPLATE = lib

CONFIG += c++14

DESTDIR = $${OUT_PWD}/../lib

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000 QT_MESSAGELOGCONTEXT
DEFINES += ZEITERFASSUNGLIB_LIBRARY

SOURCES += zeiterfassungapi.cpp \
           zeiterfassungplugin.cpp \
           replies/createtimeassignmentreply.cpp \
           replies/updatebookingreply.cpp \
           replies/getbookingsreply.cpp \
           replies/getpresencestatusreply.cpp \
           replies/gettimeassignmentsreply.cpp \
           replies/deletetimeassignmentreply.cpp \
           replies/loginpagereply.cpp \
           replies/getprojectsreply.cpp \
           replies/getauswertungreply.cpp \
           replies/loginreply.cpp \
           replies/userinforeply.cpp \
           replies/zeiterfassungreply.cpp \
           replies/updatetimeassignmentreply.cpp \
           replies/deletebookingreply.cpp \
           replies/createbookingreply.cpp

HEADERS += cpp14polyfills.h \
           zeiterfassunglib_global.h \
           zeiterfassungapi.h \
           zeiterfassungplugin.h \
           replies/createtimeassignmentreply.h \
           replies/updatebookingreply.h \
           replies/getbookingsreply.h \
           replies/getpresencestatusreply.h \
           replies/gettimeassignmentsreply.h \
           replies/deletetimeassignmentreply.h \
           replies/loginpagereply.h \
           replies/getprojectsreply.h \
           replies/getauswertungreply.h \
           replies/loginreply.h \
           replies/userinforeply.h \
           replies/zeiterfassungreply.h \
           replies/updatetimeassignmentreply.h \
           replies/deletebookingreply.h \
           replies/createbookingreply.h

TRANSLATIONS += \
    translations/zeiterfassunglib_en.ts \
    translations/zeiterfassunglib_de.ts

# unix {
#    target.path = /usr/lib
#    INSTALLS += target
# }
