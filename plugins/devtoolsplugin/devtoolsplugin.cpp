#include "devtoolsplugin.h"

#include <memory>

#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QLocale>
#include <QMenu>
#include <QAction>

#include "mainwindow.h"

#include "logmodel.h"
#include "logdialog.h"

std::shared_ptr<LogModel> model;
QtMessageHandler previousHandler;

void myMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    previousHandler(type, context, message);

    model->log(type, context.file, context.line, context.function, context.category, message);
}

void registerMessageHandler()
{
    model = std::make_shared<LogModel>();
    previousHandler = qInstallMessageHandler(myMessageHandler);
}

Q_COREAPP_STARTUP_FUNCTION(registerMessageHandler)

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
    auto dialog = new LogDialog(&mainWindow);
    dialog->setModel(model.get());
    mainWindow.menuTools()->addAction(QIcon(QStringLiteral(":/zeiterfassung/plugins/devtoolsplugin/images/dev-tools.png")),
                                      tr("Show log"), dialog, &QWidget::show);
}
