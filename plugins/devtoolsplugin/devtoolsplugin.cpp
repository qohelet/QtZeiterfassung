#include "devtoolsplugin.h"

#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QLocale>

#include "mainwindow.h"

DevToolsPlugin::DevToolsPlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{
    qDebug() << "called";

    static auto dir = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(QStringLiteral("translations"));

    if(m_translator.load(QLocale(), QStringLiteral("devtoolsplugin"), QStringLiteral("_"), dir))
    {
        if(!QCoreApplication::installTranslator(&m_translator))
        {
            qWarning() << "could not install translation devtoolsplugin";
        }
    }
    else
    {
        qWarning() << "could not load translation devtoolsplugin";
    }
}

void DevToolsPlugin::attachTo(MainWindow &mainWindow)
{
    //TODO
}
