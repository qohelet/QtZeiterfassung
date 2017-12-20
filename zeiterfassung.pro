TEMPLATE = subdirs

SUBDIRS += plugins \
           zeiterfassung \
           zeiterfassungcorelib \
           zeiterfassungguilib

plugins.depends += zeiterfassungguilib
zeiterfassung.depends += zeiterfassungcorelib zeiterfassungguilib
zeiterfassungguilib.depends += zeiterfassungcorelib
