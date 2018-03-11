#include "weatherplugin.h"

#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QLocale>
#include <QStatusBar>

#include "mainwindow.h"

#include "weatherwidget.h"
#include "weathersettingswidget.h"

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

SettingsWidget *WeatherPlugin::settingsWidget(ZeiterfassungSettings &settings, QWidget *parent) const
{
    return new WeatherSettingsWidget(settings, parent);
}
