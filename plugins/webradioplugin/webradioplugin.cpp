#include "webradioplugin.h"

#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QLocale>
#include <QDialog>
#include <QMenu>
#include <QAction>

#include "mainwindow.h"

WebRadioPlugin::WebRadioPlugin(QObject *parent) :
    ZeiterfassungPlugin(parent)
{
    qDebug() << "called";

    static auto dir = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(QStringLiteral("translations"));

    if(m_translator.load(QLocale(), QStringLiteral("webradioplugin"), QStringLiteral("_"), dir))
    {
        if(!QCoreApplication::installTranslator(&m_translator))
        {
            qWarning() << "could not install translation webradioplugin";
        }
    }
    else
    {
        qWarning() << "could not load translation webradioplugin";
    }
}

void WebRadioPlugin::attachTo(MainWindow &mainWindow)
{
    auto dialog = new QDialog(&mainWindow);
    dialog->setWindowFlag(Qt::WindowCloseButtonHint, true);
    dialog->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    mainWindow.menuTools()->addAction(QIcon(QStringLiteral(":/zeiterfassung/plugins/webradioplugin/images/web-radio.png")),
                                      tr("Play webradio"), dialog, &QWidget::show);
}
