#include "lunchmealplugin.h"

#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QLocale>
#include <QBoxLayout>

#include "mainwindow.h"
#include "stripswidget.h"

#include "lunchmealwidget.h"

LunchMealPlugin::LunchMealPlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{
    qDebug() << "called";

    static auto dir = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(QStringLiteral("translations"));

    if(m_translator.load(QLocale(), QStringLiteral("lunchmealplugin"), QStringLiteral("_"), dir))
    {
        if(!QCoreApplication::installTranslator(&m_translator))
        {
            qWarning() << "could not install translation lunchmealplugin";
        }
    }
    else
    {
        qWarning() << "could not load translation lunchmealplugin";
    }
}

void LunchMealPlugin::attachTo(MainWindow &mainWindow)
{
    for(auto stripsWidget : mainWindow.stripsWidgets())
        stripsWidget->headerLayout()->addWidget(new LunchMealWidget(*stripsWidget));
}
