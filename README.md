# Zeiterfassung
This tool helps me assigning my working hours to projects at work.

[![Build Status](https://travis-ci.org/0xFEEDC0DE64/QtZeiterfassung.svg?branch=master)](https://travis-ci.org/0xFEEDC0DE64/QtZeiterfassung)

![Screenshot of the main window](https://raw.githubusercontent.com/0xFEEDC0DE64/QtZeiterfassung/master/screenshot.png)

# Getting the tool up and running
Getting prebuilt binaries:
1. [Launching the tool with docker](https://github.com/0xFEEDC0DE64/QtZeiterfassung/wiki/Launching-the-tool-with-docker)
2. Download the [latest release](https://github.com/0xFEEDC0DE64/QtZeiterfassung/releases) and use the start script

Building from source:
1. [Building the docker image yourself](https://github.com/0xFEEDC0DE64/QtZeiterfassung/wiki/Building-the-docker-image-yourself)
2. [Building in QtCreator](https://github.com/0xFEEDC0DE64/QtZeiterfassung/wiki/Building-in-QtCreator-on-Windows)
3. [Building from source](https://github.com/0xFEEDC0DE64/QtZeiterfassung/wiki/Building-from-source)

## Configuration
This tool saves its configuration using [QSettings](https://doc.qt.io/qt-5/qsettings.html). On linux, the configuration files are placed in `~/.config/db-software/zeiterfassung.conf`. **Be careful!** This config file contains your password in plain text (if you log in correctly). You can alter the code in main.cpp to change the behaviour of QSettings (for example: saving into an ini file at working directory).
