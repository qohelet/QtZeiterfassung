#include "updaterplugin.h"

#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QLocale>

#include "mainwindow.h"
#include "zeiterfassungsettings.h"
#include "zeiterfassungapi.h"

#include "updatersettings.h"
#include "updaterdialog.h"

UpdaterPlugin::UpdaterPlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{
    qDebug() << "called";

    static auto dir = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(QStringLiteral("translations"));

    if(m_translator.load(QLocale(), QStringLiteral("updaterplugin"), QStringLiteral("_"), dir))
    {
        if(!QCoreApplication::installTranslator(&m_translator))
        {
            qWarning() << "could not install translation updaterplugin";
        }
    }
    else
    {
        qWarning() << "could not load translation updaterplugin";
    }
}

void UpdaterPlugin::attachTo(MainWindow &mainWindow)
{
    auto lastUpdateCheck = UpdaterSettings(mainWindow.settings()).lastUpdateCheck();
    if(lastUpdateCheck.isNull() || lastUpdateCheck < QDate::currentDate())
        new UpdaterDialog(mainWindow);
}
