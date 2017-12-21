#include "profileplugin.h"

#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QLocale>
#include <QDialog>
#include <QMenu>
#include <QAction>

#include "mainwindow.h"

ProfilePlugin::ProfilePlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{
    qDebug() << "called";

    static auto dir = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(QStringLiteral("translations"));

    if(m_translator.load(QLocale(), QStringLiteral("profileplugin"), QStringLiteral("_"), dir))
    {
        if(!QCoreApplication::installTranslator(&m_translator))
        {
            qWarning() << "could not install translation profileplugin";
        }
    }
    else
    {
        qWarning() << "could not load translation profileplugin";
    }
}

void ProfilePlugin::attachTo(MainWindow &mainWindow)
{
    auto dialog = new QDialog(&mainWindow);
    mainWindow.menuTools()->addAction(QIcon(QStringLiteral(":/zeiterfassung/plugins/profileplugin/images/profile.png")),
                                      tr("My profile"), dialog, &QDialog::open);
}
