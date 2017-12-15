CONFIG(debug, release|debug): DEBUG_SIGN = d

libinstall.path = $${DESTDIR}
libinstall.files = win32/Qt.conf \
                   $$OUT_PWD/../lib/zeiterfassunglib.dll \
                   $$[QT_INSTALL_BINS]/Qt5Core$${DEBUG_SIGN}.dll \
                   $$[QT_INSTALL_BINS]/Qt5Gui$${DEBUG_SIGN}.dll \
                   $$[QT_INSTALL_BINS]/Qt5Network$${DEBUG_SIGN}.dll \
                   $$[QT_INSTALL_BINS]/Qt5Widgets$${DEBUG_SIGN}.dll \
                   $$[QT_INSTALL_BINS]/libgcc_s_dw2-1.dll \
                   $$[QT_INSTALL_BINS]/libstd~1.dll \
                   $$[QT_INSTALL_BINS]/libwinpthread-1.dll
INSTALLS += libinstall

iconenginesinstall.path = $$DESTDIR/plugins/iconengines
iconenginesinstall.files = $$[QT_INSTALL_PLUGINS]/iconengines/qsvgicon$${DEBUG_SIGN}.dll

imageformatsinstall.path = $$DESTDIR/plugins/imageformats
imageformatsinstall.files = $$[QT_INSTALL_PLUGINS]/imageformats/qdds$${DEBUG_SIGN}.dll \
                            $$[QT_INSTALL_PLUGINS]/imageformats/qgif$${DEBUG_SIGN}.dll \
                            $$[QT_INSTALL_PLUGINS]/imageformats/qicns$${DEBUG_SIGN}.dll \
                            $$[QT_INSTALL_PLUGINS]/imageformats/qico$${DEBUG_SIGN}.dll \
                            $$[QT_INSTALL_PLUGINS]/imageformats/qjpeg$${DEBUG_SIGN}.dll \
                            $$[QT_INSTALL_PLUGINS]/imageformats/qsvg$${DEBUG_SIGN}.dll \
                            $$[QT_INSTALL_PLUGINS]/imageformats/qtga$${DEBUG_SIGN}.dll \
                            $$[QT_INSTALL_PLUGINS]/imageformats/qtiff$${DEBUG_SIGN}.dll \
                            $$[QT_INSTALL_PLUGINS]/imageformats/qwbmp$${DEBUG_SIGN}.dll \
                            $$[QT_INSTALL_PLUGINS]/imageformats/qwebp$${DEBUG_SIGN}.dll

platformsinstall.path = $$DESTDIR/plugins/platforms
win32: platformsinstall.files = $$[QT_INSTALL_PLUGINS]/platforms/qwindows$${DEBUG_SIGN}.dll
