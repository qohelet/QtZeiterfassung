#include "presenceplugin.h"

#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QLocale>
#include <QStatusBar>

#include "mainwindow.h"

#include "presencewidget.h"
#include "presencesettingswidget.h"

PresencePlugin::PresencePlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{
    qDebug() << "called";

    static auto dir = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(QStringLiteral("translations"));

    if(m_translator.load(QLocale(), QStringLiteral("presenceplugin"), QStringLiteral("_"), dir))
    {
        if(!QCoreApplication::installTranslator(&m_translator))
        {
            qWarning() << "could not install translation presenceplugin";
        }
    }
    else
    {
        qWarning() << "could not load translation presenceplugin";
    }
}

void PresencePlugin::attachTo(MainWindow &mainWindow)
{
    mainWindow.statusBar()->addWidget(new PresenceWidget(mainWindow));
}

SettingsWidget *PresencePlugin::settingsWidget(ZeiterfassungSettings &settings, QWidget *parent) const
{
    return new PresenceSettingsWidget(settings, parent);
}
