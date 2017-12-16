TEMPLATE = subdirs

SUBDIRS += plugins \
           zeiterfassung \
           zeiterfassunglib

plugins.depends += zeiterfassunglib
zeiterfassung.depends += zeiterfassunglib
