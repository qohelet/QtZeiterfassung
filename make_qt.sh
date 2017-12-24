#!/bin/bash0
git clone --branch=5.10 git://code.qt.io/qt/qt5.git
pushd qt5
perl init-repository --module-subset=qtbase,qtimageformats
./configure -static -opensource -nomake examples -nomake tests -confirm-license
make -j2
sudo make install
popd