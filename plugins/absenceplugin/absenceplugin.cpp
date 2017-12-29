#include "absenceplugin.h"

#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QLocale>
#include <QBoxLayout>

#include "mainwindow.h"
#include "stripswidget.h"

#include "absencewidget.h"

AbsencePlugin::AbsencePlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{
    qDebug() << "called";

    static auto dir = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(QStringLiteral("translations"));

    if(m_translator.load(QLocale(), QStringLiteral("absenceplugin"), QStringLiteral("_"), dir))
    {
        if(!QCoreApplication::installTranslator(&m_translator))
        {
            qWarning() << "could not install translation absenceplugin";
        }
    }
    else
    {
        qWarning() << "could not load translation absenceplugin";
    }
}

void AbsencePlugin::attachTo(MainWindow &mainWindow)
{
    for(auto stripsWidget : mainWindow.stripsWidgets())
        stripsWidget->headerLayout()->addWidget(new AbsenceWidget(*stripsWidget));
}
