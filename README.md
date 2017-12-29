# Zeiterfassung
This tool helps me assigning my working hours to projects at work.

[![Build Status](https://travis-ci.org/0xFEEDC0DE64/QtZeiterfassung.svg?branch=master)](https://travis-ci.org/0xFEEDC0DE64/QtZeiterfassung)

![Screenshot of the main window](https://raw.githubusercontent.com/0xFEEDC0DE64/QtZeiterfassung/master/screenshot.png)

## Building from source
The build process has only been tested with gcc. On windows you have to use MinGW (provided by the Qt setup). All necessary config files or translations should be copied over to the build folder. The executable with all plugin lands in your build folder under /bin

The simplest way to get it up and running is to just open it in QtCreator. If you are more like a terminal monkey, you can build it there too:
```
git clone https://github.com/0xFEEDC0DE64/QtZeiterfassung.git
mkdir build_QtZeiterfassung
cd build_QtZeiterfassung
qmake ../QtZeiterfassung
make
make install # only needed on first build to copy Qt resources
```

## Launching (on unix)
```
./zeiterfassung.sh
```

## Launching (on win32)
Double click the **zeiterfassung.exe**. Please report any error message like missing libraries or plugins!

## Configuration
This tool saves its configuration using [QSettings](https://doc.qt.io/qt-5/qsettings.html). On linux, the configuration files are placed in `~/.config/db-software/zeiterfassung.conf`. **Be careful!** This config file contains your password in plain text (if you log in correctly). You can alter the code in main.cpp to change the behaviour of QSettings (for example: saving into an ini file at working directory).
