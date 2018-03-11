#include <memory>

#include <QApplication>
#include <QTranslator>
#include <QMessageBox>
#include <QSplashScreen>
#include <QPixmap>
#include <QLocale>
#include <QTranslator>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QInputDialog>
#include <QStringBuilder>
#include <QLibrary>
#include <QPluginLoader>
#include <QDebug>

#include "zeiterfassungsettings.h"
#include "dialogs/languageselectiondialog.h"
#include "zeiterfassungapi.h"
#include "dialogs/authenticationdialog.h"
#include "zeiterfassungplugin.h"
#include "mainwindow.h"
#include "replies/loginpagereply.h"
#include "replies/loginreply.h"
#include "replies/getuserinforeply.h"
#include "stripfactory.h"

struct {
    QTranslator qtTranslator;
    QTranslator zeiterfassungTranslator;
    QTranslator zeiterfassungguilibTranslator;
} translators;

QSet<std::shared_ptr<QPluginLoader> > pluginLoaders;
QSet<ZeiterfassungPlugin*> plugins;

bool loadAndInstallTranslator(QTranslator &translator, const QString &filename)
{
    static auto dir = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(QStringLiteral("translations"));

    if(!translator.load(QLocale(), filename, QStringLiteral("_"), dir))
    {
        qWarning() << "could not load translation" << filename;
        return false;
    }

    if(!QCoreApplication::installTranslator(&translator))
    {
        qWarning() << "could not install translation" << filename;
        return false;
    }

    return true;
}

bool loadTranslations(QSplashScreen &splashScreen, ZeiterfassungSettings &settings)
{
    splashScreen.showMessage(QCoreApplication::translate("main", "Loading translations..."), Qt::AlignHCenter | Qt::AlignBottom);

    if(settings.language() == QLocale::AnyLanguage)
    {
        LanguageSelectionDialog dialog(&splashScreen);

        again:
        if(dialog.exec() != QDialog::Accepted)
            return false;

        if(dialog.language() == QLocale::AnyLanguage)
        {
            QMessageBox::warning(&splashScreen, QCoreApplication::translate("main", "Invalid language selection!"),
                                 QCoreApplication::translate("main", "Invalid language selection!") % "\n\n" %
                                 QCoreApplication::translate("main", "You did not select a valid language!"));
            goto again;
        }

        settings.setLanguage(dialog.language());
    }

    QLocale::setDefault(QLocale(settings.language(), QLocale::Austria));

    loadAndInstallTranslator(translators.qtTranslator,               QStringLiteral("qt"));
    loadAndInstallTranslator(translators.zeiterfassungTranslator,    QStringLiteral("zeiterfassung"));
    loadAndInstallTranslator(translators.zeiterfassungguilibTranslator, QStringLiteral("zeiterfassungguilib"));

    return true;
}

bool loadTheme(QSplashScreen &splashScreen, ZeiterfassungSettings &settings)
{
    splashScreen.showMessage(QCoreApplication::translate("main", "Loading theme..."), Qt::AlignHCenter | Qt::AlignBottom);

    if(settings.theme().isEmpty())
        return true;

    auto themePath = QDir(QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(QStringLiteral("themes"))).absoluteFilePath(settings.theme());

    QFile file(themePath % ".qss");

    if(!file.exists())
    {
        QMessageBox::warning(&splashScreen, QCoreApplication::translate("main", "Could not load theme!"),
                             QCoreApplication::translate("main", "Could not load theme!") % "\n\n" %
                             QCoreApplication::translate("main", "Theme file does not exist!"));
        return false;
    }

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(&splashScreen, QCoreApplication::translate("main", "Could not load theme!"),
                             QCoreApplication::translate("main", "Could not load theme!") % "\n\n" %
                             file.errorString());
        return false;
    }

    QTextStream textStream(&file);
    qApp->setStyleSheet(textStream.readAll().replace(QStringLiteral("@THEME_RESOURCES@"), themePath));

    return true;
}

bool loadStripLayouts(QSplashScreen &splashScreen, StripFactory &stripFactory)
{
    splashScreen.showMessage(QCoreApplication::translate("main", "Loading strip layouts..."), Qt::AlignHCenter | Qt::AlignBottom);

    if(!stripFactory.load(QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(QStringLiteral("strips"))))
    {
        QMessageBox::warning(&splashScreen, QCoreApplication::translate("main", "Could not load strips!"),
                             QCoreApplication::translate("main", "Could not load strips!") % "\n\n" % stripFactory.errorString());
        return false;
    }

    {
        auto widget = stripFactory.createBookingStartStrip();
        if(!widget)
        {
            QMessageBox::warning(&splashScreen, QCoreApplication::translate("main", "Could not load strips!"),
                                 QCoreApplication::translate("main", "Could not load strips!") % "\n\n" % stripFactory.errorString());
            return false;
        }
    }

    {
        auto widget = stripFactory.createBookingEndStrip();
        if(!widget)
        {
            QMessageBox::warning(&splashScreen, QCoreApplication::translate("main", "Could not load strips!"),
                                 QCoreApplication::translate("main", "Could not load strips!") % "\n\n" % stripFactory.errorString());
            return false;
        }
    }

    {
        auto widget = stripFactory.createTimeAssignmentStrip();
        if(!widget)
        {
            QMessageBox::warning(&splashScreen, QCoreApplication::translate("main", "Could not load strips!"),
                                 QCoreApplication::translate("main", "Could not load strips!") % "\n\n" % stripFactory.errorString());
            return false;
        }
    }

    return true;
}

bool loadLoginPage(QSplashScreen &splashScreen, ZeiterfassungSettings &settings, ZeiterfassungApi &erfassung)
{
    splashScreen.showMessage(QCoreApplication::translate("main", "Loading login page..."), Qt::AlignHCenter | Qt::AlignBottom);

    again:
    auto reply = erfassung.doLoginPage();

    reply->waitForFinished();

    if(!reply->success())
    {
        QMessageBox::warning(&splashScreen, QCoreApplication::translate("main", "Could not access Zeiterfassung!"),
                             QCoreApplication::translate("main", "Could not access Zeiterfassung!") % "\n\n" % reply->message());

        bool ok;
        auto url = QInputDialog::getText(&splashScreen, QCoreApplication::translate("main", "Base url"),
                                         QCoreApplication::translate("main", "Please enter the base url to the Zeiterfassung:"),
                                         QLineEdit::Normal, settings.url(), &ok);

        if(!ok)
            return false;

        settings.setUrl(url);
        erfassung.setUrl(url);

        goto again;
    }

    return true;
}

bool doAuthentication(QSplashScreen &splashScreen, ZeiterfassungSettings &settings, ZeiterfassungApi &erfassung)
{
    splashScreen.showMessage(QCoreApplication::translate("main", "Authenticating..."), Qt::AlignHCenter | Qt::AlignBottom);

    if(settings.username().isNull() || settings.password().isNull())
    {
        AuthenticationDialog dialog(&splashScreen);

        if(dialog.exec() != QDialog::Accepted)
            return false;

        settings.setUsername(dialog.username());
        settings.setPassword(dialog.password());
    }

    {
        again:
        auto reply = erfassung.doLogin(settings.username(), settings.password());

        reply->waitForFinished();

        if(!reply->success())
        {
            QMessageBox::warning(&splashScreen, QCoreApplication::translate("main", "Could not authenticate with Zeiterfassung!"),
                                 QCoreApplication::translate("main", "Could not authenticate with Zeiterfassung!") % "\n\n" % reply->message());

            AuthenticationDialog dialog(&splashScreen);
            dialog.setUsername(settings.username());
            dialog.setPassword(settings.password());

            if(dialog.exec() != QDialog::Accepted)
                return false;

            settings.setUsername(dialog.username());
            settings.setPassword(dialog.password());

            goto again;
        }
    }

    return true;
}

bool loadUserInfo(QSplashScreen &splashScreen, ZeiterfassungApi &erfassung, GetUserInfoReply::UserInfo &userInfo)
{
    splashScreen.showMessage(QCoreApplication::translate("main", "Getting user information..."), Qt::AlignHCenter | Qt::AlignBottom);

    {
        auto reply = erfassung.doUserInfo();

        reply->waitForFinished();

        if(!reply->success())
        {
            QMessageBox::warning(&splashScreen, QCoreApplication::translate("main", "Could not get user information!"),
                                 QCoreApplication::translate("main", "Could not get user information!") % "\n\n" % reply->message());
            return false;
        }

        userInfo = reply->userInfo();
    }

    return true;
}

bool loadPlugins(QSplashScreen &splashScreen)
{
    auto ok = true;

    QDir dir(
        QDir(
            QDir(
                QCoreApplication::applicationDirPath()
            ).absoluteFilePath(QStringLiteral("plugins"))
        ).absoluteFilePath(QStringLiteral("zeiterfassung"))
    );

    for(const auto &fileInfo : dir.entryInfoList(QDir::Files | QDir::NoSymLinks))
    {
        if(!QLibrary::isLibrary(fileInfo.filePath()))
        {
            qWarning() << "skipping" << fileInfo.fileName() << "because no QLibrary";
            continue; // to skip windows junk files
        }

        auto pluginLoader = std::make_shared<QPluginLoader>(fileInfo.filePath());
        if(!pluginLoader->load())
        {
            QMessageBox::warning(&splashScreen, QCoreApplication::translate("main", "Could not load plugin %0!"),
                                 QCoreApplication::translate("main", "Could not load plugin %0!").arg(fileInfo.fileName()) %
                                 "\n\n" % pluginLoader->errorString());
            ok = false;
            continue;
        }

        if(auto plugin = qobject_cast<ZeiterfassungPlugin*>(pluginLoader->instance()))
        {
            pluginLoaders.insert(pluginLoader);
            plugins.insert(plugin);
        }
        else
            QMessageBox::warning(&splashScreen, QCoreApplication::translate("main", "Plugin not valid %0!"),
                                 QCoreApplication::translate("main", "Plugin not valid %0!").arg(pluginLoader->fileName()) %
                                 "\n\n" % pluginLoader->errorString());

    }

    return ok;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qSetMessagePattern(QStringLiteral("%{time dd.MM.yyyy HH:mm:ss.zzz} "
                                      "["
                                      "%{if-debug}D%{endif}"
                                      "%{if-info}I%{endif}"
                                      "%{if-warning}W%{endif}"
                                      "%{if-critical}C%{endif}"
                                      "%{if-fatal}F%{endif}"
                                      "] "
                                      "%{function}(): "
                                      "%{message}"));

    QCoreApplication::setOrganizationDomain(QStringLiteral("brunner.ninja"));
    QCoreApplication::setOrganizationName(QStringLiteral("db-software"));
    QCoreApplication::setApplicationName(QStringLiteral("zeiterfassung"));
    QCoreApplication::setApplicationVersion(QStringLiteral("1.5"));

    QSplashScreen splashScreen(QPixmap(QStringLiteral(":/zeiterfassung/images/splash.png")));
    splashScreen.showMessage(QCoreApplication::translate("main", "Loading settings..."), Qt::AlignHCenter | Qt::AlignBottom);
    splashScreen.show();

    ZeiterfassungSettings settings(&app);

    if(!loadTranslations(splashScreen, settings))
        return -1;

    // not critical if it fails
    //if(!loadTheme(splashScreen, settings))
    //    return -2;
    loadTheme(splashScreen, settings);

    StripFactory stripFactory(&app);

    if(!loadStripLayouts(splashScreen, stripFactory))
        return -3;

    ZeiterfassungApi erfassung(settings.url(), &app);

    if(!loadLoginPage(splashScreen, settings, erfassung))
        return -4;

    if(!doAuthentication(splashScreen, settings, erfassung))
        return -5;

    GetUserInfoReply::UserInfo userInfo;

    if(!loadUserInfo(splashScreen, erfassung, userInfo))
        return -6;

    loadPlugins(splashScreen);

    MainWindow mainWindow(settings, erfassung, userInfo, stripFactory, plugins);
    splashScreen.finish(&mainWindow);

    for(auto &plugin : plugins)
        plugin->attachTo(mainWindow);

    mainWindow.show();

    return app.exec();
}
