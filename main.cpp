#include <QApplication>
#include <QTranslator>
#include <QMessageBox>
#include <QSplashScreen>
#include <QPixmap>
#include <QDir>
#include <QInputDialog>
#include <QStringBuilder>
#include <QDebug>

#include "zeiterfassungsettings.h"
#include "dialogs/languageselectiondialog.h"
#include "zeiterfassung.h"
#include "eventloopwithstatus.h"
#include "dialogs/authenticationdialog.h"
#include "mainwindow.h"

bool loadAndInstallTranslator(QTranslator &translator,
                              const QLocale &locale,
                              const QString &filename,
                              const QString &prefix = QString(),
                              const QString &directory = QString(),
                              const QString &suffix = QString())
{
    if(!translator.load(locale, filename, prefix, directory, suffix))
    {
        qDebug() << "could not load translation" << filename;
        return false;
    }

    if(!QCoreApplication::installTranslator(&translator))
    {
        qDebug() << "could not install translation" << filename;
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
                                 QCoreApplication::translate("main", "You did not select a valid language."));
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
    loadAndInstallTranslator(zeiterfassungTranslator, locale, QStringLiteral("zeiterfassung"),
                             QStringLiteral("_"), QStringLiteral(":/zeiterfassung/translations"));

    splashScreen.showMessage(QCoreApplication::translate("main", "Loading login page..."));

    Zeiterfassung erfassung(settings.url(), &app);

    {
        EventLoopWithStatus eventLoop;
        QObject::connect(&erfassung, &Zeiterfassung::loginPageFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

        again1:
        erfassung.doLoginPage();
        eventLoop.exec();

        if(!eventLoop.success())
        {
            bool ok;
            QMessageBox::warning(&splashScreen, QCoreApplication::translate("main", "Could not access Zeiterfassung!"),
                                 QCoreApplication::translate("main", "Could not access Zeiterfassung!") % "\n\n" % eventLoop.message());

            auto url = QInputDialog::getText(&splashScreen, QCoreApplication::translate("main", "Base url"),
                                             QCoreApplication::translate("main", "Please enter the base url to the Zeiterfassung:"),
                                             QLineEdit::Normal, settings.url(), &ok);
            if(!ok)
                return -1;
            settings.setUrl(url);
            erfassung.setUrl(url);
            goto again1;
        }
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
        EventLoopWithStatus eventLoop;
        QObject::connect(&erfassung, &Zeiterfassung::loginFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

        again2:
        erfassung.doLogin(settings.username(), settings.password());
        eventLoop.exec();

        if(!eventLoop.success())
        {
            QMessageBox::warning(&splashScreen, QCoreApplication::translate("main", "Could not authenticate with Zeiterfassung!"),
                                 QCoreApplication::translate("main", "Could not authenticate with Zeiterfassung!") % "\n\n" % eventLoop.message());

            AuthenticationDialog dialog(&splashScreen);
            dialog.setUsername(settings.username());
            dialog.setPassword(settings.password());
            if(dialog.exec() != QDialog::Accepted)
                return -1;
            settings.setUsername(dialog.username());
            settings.setPassword(dialog.password());

            goto again2;
        }
    }

    splashScreen.showMessage(QCoreApplication::translate("main", "Getting user information..."));

    Zeiterfassung::UserInfo userInfo;

    {
        EventLoopWithStatus eventLoop;
        QObject::connect(&erfassung, &Zeiterfassung::userInfoFinished,
                         [&](bool success, const QString &message, const Zeiterfassung::UserInfo &_userInfo) {
            Q_UNUSED(message)
            if(success)
                userInfo = _userInfo;
        });
        QObject::connect(&erfassung, &Zeiterfassung::userInfoFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

        erfassung.doUserInfo();
        eventLoop.exec();

        if(!eventLoop.success())
        {
            QMessageBox::warning(&splashScreen, QCoreApplication::translate("main", "Could not get user information!"),
                                 QCoreApplication::translate("main", "Could not get user information!") % "\n\n" % eventLoop.message());

            return -1;
        }
    }

    MainWindow mainWindow(settings, erfassung, userInfo);
    mainWindow.show();

    splashScreen.finish(&mainWindow);

    return app.exec();
}
