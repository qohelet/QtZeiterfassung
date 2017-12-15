translationsinstall.path = $${DESTDIR}/translations
translationsinstall.files = $$[QT_INSTALL_TRANSLATIONS]/qt_en.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qtbase_en.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qtmultimedia_en.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qtquick1_en.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qtscript_nen.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_en.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qt_de.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qtbase_de.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qtmultimedia_de.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qtquick1_de.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qtscript_de.qm \
                            $$[QT_INSTALL_TRANSLATIONS]/qtxmlpatterns_de.qm \
                            translations/zeiterfassung_en.qm \
                            translations/zeiterfassung_de.qm \
                            ../zeiterfassunglib/translations/zeiterfassunglib_en.qm \
                            ../zeiterfassunglib/translations/zeiterfassunglib_de.qm
INSTALLS += translationsinstall

themesinstall.path = $${DESTDIR}/themes
themesinstall.files = themes/dark_theme.qss
INSTALLS += themesinstall

darkthemeinstall.path = $${DESTDIR}/themes/dark_theme
darkthemeinstall.files = themes/dark_theme/checkbox_indeterminate_disabled.png \
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
INSTALLS += darkthemeinstall

stripsinstall.path = $${DESTDIR}/strips
stripsinstall.files = strips/bookingstartstrip.ui \
                      strips/bookingendstrip.ui \
                      strips/timeassignmentstrip.ui
INSTALLS += stripsinstall
