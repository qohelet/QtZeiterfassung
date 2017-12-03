#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QInputDialog>
#include <QMessageBox>

#include "zeiterfassungsettings.h"
#include "zeiterfassung.h"
#include "eventloopwithstatus.h"
#include "dialogs/authenticationdialog.h"
#include "mainwindow.h"

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

    QLocale::setDefault(QLocale(QLocale::German, QLocale::Austria));

    QCoreApplication::setOrganizationDomain(QStringLiteral("brunner.ninja"));
    QCoreApplication::setOrganizationName(QStringLiteral("db-software"));
    QCoreApplication::setApplicationName(QStringLiteral("zeiterfassung"));
    QCoreApplication::setApplicationVersion(QStringLiteral("1.0"));

    QSplashScreen splashScreen(QPixmap(":/zeiterfassung/images/splash.png"));
    splashScreen.showMessage(QObject::tr("Loading settings..."));
    splashScreen.show();

    ZeiterfassungSettings settings(&app);

    splashScreen.showMessage(QObject::tr("Loading login page..."));

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
            QMessageBox::warning(&splashScreen, QObject::tr("Could not access Zeiterfassung"),
                                 QObject::tr("The Zeiterfassung could not be accessed:\n\n%0").arg(eventLoop.message()));

            auto url = QInputDialog::getText(&splashScreen, QObject::tr("Base url"),
                                             QObject::tr("Please enter the base url to the Zeiterfassung:"),
                                             QLineEdit::Normal, settings.url(), &ok);
            if(!ok)
                return -1;
            settings.setUrl(url);
            erfassung.setUrl(url);
            goto again1;
        }
    }

    splashScreen.showMessage(QObject::tr("Authenticating..."));

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
            QMessageBox::warning(&splashScreen, QObject::tr("Could not authenticate with Zeiterfassung"),
                                 QObject::tr("The Zeiterfassung authentication was not successful:\n\n%0").arg(eventLoop.message()));

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

    splashScreen.showMessage(QObject::tr("Getting user information..."));

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
            QMessageBox::warning(&splashScreen, QObject::tr("Could not get user information!"),
                                 QObject::tr("Could not get user information:\n\n%0").arg(eventLoop.message()));

            return -1;
        }
    }

    MainWindow mainWindow(settings, erfassung, userInfo);
    mainWindow.show();

    splashScreen.finish(&mainWindow);

    return app.exec();
}
