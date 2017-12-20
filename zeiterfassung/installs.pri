COMPILED_TRANSLATIONS += $$[QT_INSTALL_TRANSLATIONS]/qt_en.qm \
                         $$[QT_INSTALL_TRANSLATIONS]/qtbase_en.qm \
                         $$[QT_INSTALL_TRANSLATIONS]/qtmultimedia_en.qm \
                         $$[QT_INSTALL_TRANSLATIONS]/qtquick1_en.qm \
                         $$[QT_INSTALL_TRANSLATIONS]/qtscript_en.qm \
                         $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_en.qm \
                         $$[QT_INSTALL_TRANSLATIONS]/qt_de.qm \
                         $$[QT_INSTALL_TRANSLATIONS]/qtbase_de.qm \
                         $$[QT_INSTALL_TRANSLATIONS]/qtmultimedia_de.qm \
                         $$[QT_INSTALL_TRANSLATIONS]/qtquick1_de.qm \
                         $$[QT_INSTALL_TRANSLATIONS]/qtscript_de.qm \
                         $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_de.qm \
                         $${OUT_PWD}/translations/zeiterfassung_en.qm \
                         $${OUT_PWD}/translations/zeiterfassung_de.qm \
                         $${OUT_PWD}/../zeiterfassungcorelib/translations/zeiterfassungcorelib_en.qm \
                         $${OUT_PWD}/../zeiterfassungcorelib/translations/zeiterfassungcorelib_de.qm \
                         $${OUT_PWD}/../zeiterfassungguilib/translations/zeiterfassungguilib_en.qm \
                         $${OUT_PWD}/../zeiterfassungguilib/translations/zeiterfassungguilib_de.qm

copy_compiled_translations.input = COMPILED_TRANSLATIONS
copy_compiled_translations.output = $${DESTDIR}/translations/${QMAKE_FILE_BASE}${QMAKE_FILE_EXT}
copy_compiled_translations.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
copy_compiled_translations.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += copy_compiled_translations
PRE_TARGETDEPS += compiler_copy_compiled_translations_make_all

THEMES += themes/dark_theme.qss

copy_themes.input = THEMES
copy_themes.output = $${DESTDIR}/themes/${QMAKE_FILE_BASE}${QMAKE_FILE_EXT}
copy_themes.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
copy_themes.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += copy_themes
PRE_TARGETDEPS += compiler_copy_themes_make_all

DARK_THEME_RESOURCES += themes/dark_theme/checkbox_indeterminate_disabled.png \
                        themes/dark_theme/radio_unchecked.png \
                        themes/dark_theme/up_arrow.png \
                        themes/dark_theme/branch_closed-on.png \
                        themes/dark_theme/checkbox_checked_disabled.png \
                        themes/dark_theme/checkbox_unchecked.png \
                        themes/dark_theme/checkbox_indeterminate.png \
                        themes/dark_theme/stylesheet-branch-more.png \
                        themes/dark_theme/checkbox_checked.png \
                        themes/dark_theme/checkbox_unchecked_disabled.png \
                        themes/dark_theme/radio_checked.png \
                        themes/dark_theme/checkbox_indeterminate_focus.png \
                        themes/dark_theme/checkbox_checked_focus.png \
                        themes/dark_theme/branch_closed.png \
                        themes/dark_theme/Vsepartoolbar.png \
                        themes/dark_theme/radio_checked_disabled.png \
                        themes/dark_theme/left_arrow.png \
                        themes/dark_theme/Vmovetoolbar.png \
                        themes/dark_theme/branch_open-on.png \
                        themes/dark_theme/close.png \
                        themes/dark_theme/stylesheet-branch-end.png \
                        themes/dark_theme/stylesheet-vline.png \
                        themes/dark_theme/down_arrow_disabled.png \
                        themes/dark_theme/radio_unchecked_disabled.png \
                        themes/dark_theme/left_arrow_disabled.png \
                        themes/dark_theme/Hmovetoolbar.png \
                        themes/dark_theme/close-pressed.png \
                        themes/dark_theme/up_arrow_disabled.png \
                        themes/dark_theme/branch_open.png \
                        themes/dark_theme/radio_checked_focus.png \
                        themes/dark_theme/sizegrip.png \
                        themes/dark_theme/checkbox_unchecked_focus.png \
                        themes/dark_theme/right_arrow_disabled.png \
                        themes/dark_theme/Hsepartoolbar.png \
                        themes/dark_theme/undock.png \
                        themes/dark_theme/transparent.png \
                        themes/dark_theme/close-hover.png \
                        themes/dark_theme/radio_unchecked_focus.png \
                        themes/dark_theme/down_arrow.png \
                        themes/dark_theme/right_arrow.png

copy_dark_theme_resouces.input = DARK_THEME_RESOURCES
copy_dark_theme_resouces.output = $${DESTDIR}/themes/dark_theme/${QMAKE_FILE_BASE}${QMAKE_FILE_EXT}
copy_dark_theme_resouces.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
copy_dark_theme_resouces.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += copy_dark_theme_resouces
PRE_TARGETDEPS += compiler_copy_dark_theme_resouces_make_all

STRIPLAYOUTS += strips/bookingstartstrip.ui \
                strips/bookingendstrip.ui \
                strips/timeassignmentstrip.ui

copy_striplayouts.input = STRIPLAYOUTS
copy_striplayouts.output = $${DESTDIR}/strips/${QMAKE_FILE_BASE}${QMAKE_FILE_EXT}
copy_striplayouts.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
copy_striplayouts.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += copy_striplayouts
PRE_TARGETDEPS += compiler_copy_striplayouts_make_all
