copy_compiled_translations.input = COMPILED_TRANSLATIONS
copy_compiled_translations.output = $${OUT_PWD}/../../bin/translations/${QMAKE_FILE_BASE}${QMAKE_FILE_EXT}
copy_compiled_translations.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
copy_compiled_translations.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += copy_compiled_translations
PRE_TARGETDEPS += compiler_copy_compiled_translations_make_all
