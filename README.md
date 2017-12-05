# Zeiterfassung
This tool helps me assigning my working hours to various accounts at work.

## Building from source
```
git clone https://github.com/0xFEEDC0DE64/QtZeiterfassung.git
pushd QtZeiterfassung
lrelease translations/zeiterfassung_*.ts
popd
mkdir build_QtZeiterfassung
cd build_QtZeiterfassung
qmake ../QtZeiterfassung
make
```

## Launching
```
./zeiterfassung
```

This tool saves its configuration using [QSettings](https://doc.qt.io/qt-5/qsettings.html). On linux, the configuration files are placed in `~/.config/db-software/zeiterfassung.conf`. **Be careful!** This config file contains your password in plain text (if you log in correctly). You can alter the code in main.cpp to change the behaviour of QSettings (for example: saving into an ini file at working directory).

![Screenshot of the main window](https://raw.githubusercontent.com/0xFEEDC0DE64/QtZeiterfassung/master/screenshot.png)
