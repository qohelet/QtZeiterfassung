#include "weatherplugin.h"

#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QLocale>
#include <QStatusBar>

#include "mainwindow.h"

#include "weatherwidget.h"
#include "weathersettings.h"

WeatherPlugin::WeatherPlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{
    qDebug() << "called";

    static auto dir = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(QStringLiteral("translations"));

    if(m_translator.load(QLocale(), QStringLiteral("weatherplugin"), QStringLiteral("_"), dir))
    {
        if(!QCoreApplication::installTranslator(&m_translator))
        {
            qWarning() << "could not install translation weatherplugin";
        }
    }
    else
    {
        qWarning() << "could not load translation weatherplugin";
    }
}

void WeatherPlugin::attachTo(MainWindow &mainWindow)
{
    mainWindow.statusBar()->addWidget(new WeatherWidget(mainWindow));
}

SettingsWidget *WeatherPlugin::settingsWidget(QWidget *parent)
{
    return new WeatherSettings(parent);
}
