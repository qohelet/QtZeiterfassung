themesInstall.path = $${DESTDIR}/themes
themesInstall.files = themes/dark_theme.qss
INSTALLS += themesInstall

darkThemeInstall.path = $${DESTDIR}/themes/dark_theme
darkThemeInstall.files = themes/dark_theme/checkbox_indeterminate_disabled.png \
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
INSTALLS += darkThemeInstall

stripLayoutsInstall.path = $${DESTDIR}/strips
stripLayoutsInstall.files = strips/bookingstartstrip.ui \
                            strips/bookingendstrip.ui \
                            strips/timeassignmentstrip.ui
INSTALLS += stripLayoutsInstall
