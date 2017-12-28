CONFIG(debug, release|debug): DEBUG_SIGN = d

qtLibsInstall.path = $${OUT_PWD}/bin
qtLibsInstall.files = win32/Qt.conf \
                      $$[QT_INSTALL_BINS]/Qt5Core$${DEBUG_SIGN}.dll \
                      $$[QT_INSTALL_BINS]/Qt5Gui$${DEBUG_SIGN}.dll \
                      $$[QT_INSTALL_BINS]/Qt5Network$${DEBUG_SIGN}.dll \
                      $$[QT_INSTALL_BINS]/Qt5Widgets$${DEBUG_SIGN}.dll \
                      $$[QT_INSTALL_BINS]/libgcc_s_dw2-1.dll \
                      $$[QT_INSTALL_BINS]/libstdc++-6.dll \
                      $$[QT_INSTALL_BINS]/libwinpthread-1.dll
INSTALLS += qtLibsInstall

qtImageFormatsInstall.path = $${OUT_PWD}/bin/plugins/imageformats
qtImageFormatsInstall.files = $$[QT_INSTALL_PLUGINS]/imageformats/qgif$${DEBUG_SIGN}.dll \
                              $$[QT_INSTALL_PLUGINS]/imageformats/qicns$${DEBUG_SIGN}.dll \
                              $$[QT_INSTALL_PLUGINS]/imageformats/qico$${DEBUG_SIGN}.dll \
                              $$[QT_INSTALL_PLUGINS]/imageformats/qjpeg$${DEBUG_SIGN}.dll \
                              $$[QT_INSTALL_PLUGINS]/imageformats/qsvg$${DEBUG_SIGN}.dll \
                              $$[QT_INSTALL_PLUGINS]/imageformats/qtga$${DEBUG_SIGN}.dll \
                              $$[QT_INSTALL_PLUGINS]/imageformats/qtiff$${DEBUG_SIGN}.dll \
                              $$[QT_INSTALL_PLUGINS]/imageformats/qwbmp$${DEBUG_SIGN}.dll \
                              $$[QT_INSTALL_PLUGINS]/imageformats/qwebp$${DEBUG_SIGN}.dll
INSTALLS += qtImageFormatsInstall

qtPlatformsInstall.path = $${OUT_PWD}/bin/plugins/platforms
qtPlatformsInstall.files = $$[QT_INSTALL_PLUGINS]/platforms/qwindows$${DEBUG_SIGN}.dll
INSTALLS += qtPlatformsInstall
