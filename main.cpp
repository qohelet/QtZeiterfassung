#include <QApplication>
#include <QTranslator>
#include <QEventLoop>
#include <QMessageBox>
#include <QSplashScreen>
#include <QPixmap>
#include <QDir>
#include <QInputDialog>
#include <QStringBuilder>
#include <QDebug>

#include "zeiterfassungsettings.h"
#include "dialogs/languageselectiondialog.h"
#include "zeiterfassungapi.h"
#include "dialogs/authenticationdialog.h"
#include "mainwindow.h"
#include "replies/loginpagereply.h"
#include "replies/loginreply.h"
#include "replies/userinforeply.h"

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

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qSetMessagePattern("%{time dd.MM.yyyy HH:mm:ss.zzz} "
                       "["
                       "%{if-debug}D%{endif}"
                       "%{if-info}I%{endif}"
                       "%{if-warning}W%{endif}"
                       "%{if-critical}C%{endif}"
                       "%{if-fatal}F%{endif}"
                       "] "
                       "%{function}(): "
                       "%{message}");

    QCoreApplication::setOrganizationDomain(QStringLiteral("brunner.ninja"));
    QCoreApplication::setOrganizationName(QStringLiteral("db-software"));
    QCoreApplication::setApplicationName(QStringLiteral("zeiterfassung"));
    QCoreApplication::setApplicationVersion(QStringLiteral("1.2"));

    QSplashScreen splashScreen(QPixmap(":/zeiterfassung/images/splash.png"));
    splashScreen.showMessage(QCoreApplication::translate("main", "Loading settings..."));
    splashScreen.show();

    ZeiterfassungSettings settings(&app);

    if(settings.language() == QLocale::AnyLanguage)
    {
        LanguageSelectionDialog dialog(&splashScreen);

        again0:
        if(dialog.exec() != QDialog::Accepted)
            return -1;

        if(dialog.language() == QLocale::AnyLanguage)
        {
            QMessageBox::warning(&splashScreen, QCoreApplication::translate("main", "Invalid language selection!"),
                                 QCoreApplication::translate("main", "Invalid language selection!") % "\n\n" %
                                 QCoreApplication::translate("main", "You did not select a valid language!"));
            goto again0;
        }

        settings.setLanguage(dialog.language());
    }

    QLocale locale(settings.language(), QLocale::Austria);
    QLocale::setDefault(locale);

    QTranslator qtTranslator(&app);
    loadAndInstallTranslator(qtTranslator, locale, QStringLiteral("qt"), QStringLiteral("_"),
                             QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("translations"));

    QTranslator zeiterfassungTranslator(&app);
    loadAndInstallTranslator(zeiterfassungTranslator, locale, QStringLiteral("zeiterfassung"), QStringLiteral("_"),
                             QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("translations"));

    splashScreen.showMessage(QCoreApplication::translate("main", "Loading login page..."));

    ZeiterfassungApi erfassung(settings.url(), &app);

    {
        again1:
        auto reply = erfassung.doLoginPage();

        {
            QEventLoop eventLoop;
            QObject::connect(reply, &ZeiterfassungReply::finished, &eventLoop, &QEventLoop::quit);
            eventLoop.exec();
        }

        if(!reply->success())
        {
            bool ok;
            QMessageBox::warning(&splashScreen, QCoreApplication::translate("main", "Could not access Zeiterfassung!"),
                                 QCoreApplication::translate("main", "Could not access Zeiterfassung!") % "\n\n" % reply->message());

            auto url = QInputDialog::getText(&splashScreen, QCoreApplication::translate("main", "Base url"),
                                             QCoreApplication::translate("main", "Please enter the base url to the Zeiterfassung:"),
                                             QLineEdit::Normal, settings.url(), &ok);
            if(!ok)
                return -1;
            settings.setUrl(url);
            erfassung.setUrl(url);

            reply->deleteLater();
            goto again1;
        }

        reply->deleteLater();
    }

    splashScreen.showMessage(QCoreApplication::translate("main", "Authenticating..."));

    if(settings.username().isNull() || settings.password().isNull())
    {
        AuthenticationDialog dialog(&splashScreen);
        if(dialog.exec() != QDialog::Accepted)
            return -1;
        settings.setUsername(dialog.username());
        settings.setPassword(dialog.password());
    }

    {
        again2:
        auto reply = erfassung.doLogin(settings.username(), settings.password());

        {
            QEventLoop eventLoop;
            QObject::connect(reply, &ZeiterfassungReply::finished, &eventLoop, &QEventLoop::quit);
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
                return -1;
            settings.setUsername(dialog.username());
            settings.setPassword(dialog.password());

            reply->deleteLater();
            goto again2;
        }

        reply->deleteLater();
    }

    splashScreen.showMessage(QCoreApplication::translate("main", "Getting user information..."));

    ZeiterfassungApi::UserInfo userInfo;

    {
        auto reply = erfassung.doUserInfo();

        {
            QEventLoop eventLoop;
            QObject::connect(reply, &ZeiterfassungReply::finished, &eventLoop, &QEventLoop::quit);
            eventLoop.exec();
        }

        if(!reply->success())
        {
            QMessageBox::warning(&splashScreen, QCoreApplication::translate("main", "Could not get user information!"),
                                 QCoreApplication::translate("main", "Could not get user information!") % "\n\n" % reply->message());

            reply->deleteLater();
            return -1;
        }

        userInfo = reply->userInfo();
        reply->deleteLater();
    }

    MainWindow mainWindow(settings, erfassung, userInfo);
    mainWindow.show();

    splashScreen.finish(&mainWindow);

    return app.exec();
}
