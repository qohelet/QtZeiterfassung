CONFIG(debug, release|debug): DEBUG_SIGN = d

LIBRARIES += win32/Qt.conf \
             $$OUT_PWD/../lib/zeiterfassunglib.dll \
             $$[QT_INSTALL_BINS]/Qt5Core$${DEBUG_SIGN}.dll \
             $$[QT_INSTALL_BINS]/Qt5Gui$${DEBUG_SIGN}.dll \
             $$[QT_INSTALL_BINS]/Qt5Network$${DEBUG_SIGN}.dll \
             $$[QT_INSTALL_BINS]/Qt5Widgets$${DEBUG_SIGN}.dll \
             $$[QT_INSTALL_BINS]/libgcc_s_dw2-1.dll \
             $$[QT_INSTALL_BINS]/libstdc++-6.dll \
             $$[QT_INSTALL_BINS]/libwinpthread-1.dll

copy_libraries.input = LIBRARIES
copy_libraries.output = $${DESTDIR}/${QMAKE_FILE_BASE}${QMAKE_FILE_EXT}
copy_libraries.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
copy_libraries.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += copy_libraries
PRE_TARGETDEPS += compiler_copy_libraries_make_all

IMAGE_FORMATS += $$[QT_INSTALL_PLUGINS]/imageformats/qgif$${DEBUG_SIGN}.dll \
                 $$[QT_INSTALL_PLUGINS]/imageformats/qicns$${DEBUG_SIGN}.dll \
                 $$[QT_INSTALL_PLUGINS]/imageformats/qico$${DEBUG_SIGN}.dll \
                 $$[QT_INSTALL_PLUGINS]/imageformats/qjpeg$${DEBUG_SIGN}.dll \
                 $$[QT_INSTALL_PLUGINS]/imageformats/qsvg$${DEBUG_SIGN}.dll \
                 $$[QT_INSTALL_PLUGINS]/imageformats/qtga$${DEBUG_SIGN}.dll \
                 $$[QT_INSTALL_PLUGINS]/imageformats/qtiff$${DEBUG_SIGN}.dll \
                 $$[QT_INSTALL_PLUGINS]/imageformats/qwbmp$${DEBUG_SIGN}.dll \
                 $$[QT_INSTALL_PLUGINS]/imageformats/qwebp$${DEBUG_SIGN}.dll

copy_image_formats.input = IMAGE_FORMATS
copy_image_formats.output = $${DESTDIR}/plugins/imageformats/${QMAKE_FILE_BASE}${QMAKE_FILE_EXT}
copy_image_formats.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
copy_image_formats.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += copy_image_formats
PRE_TARGETDEPS += compiler_copy_image_formats_make_all

PLATFORMS += $$[QT_INSTALL_PLUGINS]/platforms/qwindows$${DEBUG_SIGN}.dll

copy_platforms.input = PLATFORMS
copy_platforms.output = $${DESTDIR}/plugins/platforms/${QMAKE_FILE_BASE}${QMAKE_FILE_EXT}
copy_platforms.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
copy_platforms.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += copy_platforms
PRE_TARGETDEPS += compiler_copy_platforms_make_all
