TEMPLATE = subdirs

SUBDIRS += zeiterfassung \
           zeiterfassunglib

zeiterfassung.depends += zeiterfassunglib
