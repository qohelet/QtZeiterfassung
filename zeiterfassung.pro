TEMPLATE = subdirs

SUBDIRS += plugins \
           zeiterfassung \
           zeiterfassungguilib

plugins.depends += zeiterfassungguilib
zeiterfassung.depends += zeiterfassungguilib
