TEMPLATE = subdirs

SUBDIRS += plugins \
           zeiterfassung \
           zeiterfassungcorelib \
           zeiterfassungguilib

plugins.depends += zeiterfassungguilib
zeiterfassung.depends += zeiterfassungcorelib zeiterfassungguilib
zeiterfassungguilib.depends += zeiterfassungcorelib

include(installs.pri)
win32: include(installs_win32.pri)

OTHER_FILES += .travis.yml \
               Dockerfile
