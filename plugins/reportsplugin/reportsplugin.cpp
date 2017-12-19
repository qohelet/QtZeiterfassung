#include "reportsplugin.h"

#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QLocale>

#include "reportswidget.h"

ReportsPlugin::ReportsPlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{
    qDebug() << "called";

    static auto dir = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(QStringLiteral("translations"));

    if(m_translator.load(QLocale(), QStringLiteral("reportsplugin"), QStringLiteral("_"), dir))
    {
        if(!QCoreApplication::installTranslator(&m_translator))
        {
            qWarning() << "could not install translation reportsplugin";
        }
    }
    else
    {
        qWarning() << "could not load translation reportsplugin";
    }
}

void ReportsPlugin::attachTo(MainWindow &mainWindow)
{
    new ReportsWidget(mainWindow);
}
