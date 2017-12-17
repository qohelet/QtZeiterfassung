#include <QApplication>
#include <QTranslator>
#include <QEventLoop>
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
    QTranslator zeiterfassunglibTranslator;
} translators;

QVector<ZeiterfassungPlugin*> plugins;

bool loadAndInstallTranslator(QTranslator &translator,
                              const QLocale &locale,
                              const QString &filename,
                              const QString &prefix = QString(),
                              const QString &directory = QString(),
                              const QString &suffix = QString())
{
    if(!translator.load(locale, filename, prefix, directory, suffix))
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
    splashScreen.showMessage(QCoreApplication::translate("main", "Loading translations..."));

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

    QLocale locale(settings.language(), QLocale::Austria);
    QLocale::setDefault(locale);

    auto translationsDir = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath(QStringLiteral("translations"));
    loadAndInstallTranslator(translators.qtTranslator, locale, QStringLiteral("qt"), QStringLiteral("_"), translationsDir);
    loadAndInstallTranslator(translators.zeiterfassungTranslator, locale, QStringLiteral("zeiterfassung"), QStringLiteral("_"), translationsDir);
    loadAndInstallTranslator(translators.zeiterfassunglibTranslator, locale, QStringLiteral("zeiterfassunglib"), QStringLiteral("_"), translationsDir);

    return true;
}

bool loadTheme(QSplashScreen &splashScreen, ZeiterfassungSettings &settings)
{
    splashScreen.showMessage(QCoreApplication::translate("main", "Loading theme..."));

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
    splashScreen.showMessage(QCoreApplication::translate("main", "Loading strip layouts..."));

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
    splashScreen.showMessage(QCoreApplication::translate("main", "Loading login page..."));

    again:
    auto reply = erfassung.doLoginPage();

    {
        QEventLoop eventLoop;
        QObject::connect(reply.get(), &ZeiterfassungReply::finished, &eventLoop, &QEventLoop::quit);
        eventLoop.exec();
    }

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
    splashScreen.showMessage(QCoreApplication::translate("main", "Authenticating..."));

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

        {
            QEventLoop eventLoop;
            QObject::connect(reply.get(), &ZeiterfassungReply::finished, &eventLoop, &QEventLoop::quit);
            eventLoop.exec();
        }

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
    splashScreen.showMessage(QCoreApplication::translate("main", "Getting user information..."));

    {
        auto reply = erfassung.doUserInfo();

        {
            QEventLoop eventLoop;
            QObject::connect(reply.get(), &ZeiterfassungReply::finished, &eventLoop, &QEventLoop::quit);
            eventLoop.exec();
        }

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

    for(const auto &fileInfo : dir.entryInfoList(QDir::Files))
    {
        if(fileInfo.isSymLink())
            continue; // to skip unix so symlinks

        if(!QLibrary::isLibrary(fileInfo.filePath()))
            continue; // to skip windows junk files

        QPluginLoader loader(fileInfo.filePath());
        if(!loader.load())
        {
            QMessageBox::warning(&splashScreen, QCoreApplication::translate("main", "Could not load plugin %0!"),
                                 QCoreApplication::translate("main", "Could not load plugin %0!").arg(fileInfo.fileName()) %
                                 "\n\n" % loader.errorString());
            ok = false;
            continue;
        }

        auto plugin = qobject_cast<ZeiterfassungPlugin*>(loader.instance());

        if(!plugin)
        {
            QMessageBox::warning(&splashScreen, QCoreApplication::translate("main", "Plugin not valid %0!"),
                                 QCoreApplication::translate("main", "Plugin not valid %0!").arg(fileInfo.fileName()) %
                                 "\n\n" % loader.errorString());
            ok = false;
            continue;
        }

        plugins.append(plugin);
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
    QCoreApplication::setApplicationVersion(QStringLiteral("1.3.1"));

    QSplashScreen splashScreen(QPixmap(QStringLiteral(":/zeiterfassunglib/images/splash.png")));
    splashScreen.showMessage(QCoreApplication::translate("main", "Loading settings..."));
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

    MainWindow mainWindow(settings, erfassung, userInfo, stripFactory);
    splashScreen.finish(&mainWindow);

    for(auto plugin : plugins)
        plugin->attachTo(mainWindow);

    mainWindow.show();

    return app.exec();
}
