SCRIPTS += unix/start.sh

copy_scripts.input = SCRIPTS
copy_scripts.output = $${DESTDIR}/${QMAKE_FILE_BASE}${QMAKE_FILE_EXT}
copy_scripts.commands = $${QMAKE_COPY} ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
copy_scripts.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += copy_scripts
PRE_TARGETDEPS += compiler_copy_scripts_make_all
