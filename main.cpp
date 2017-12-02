#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QSettings>
#include <QInputDialog>
#include <QMessageBox>

#include "zeiterfassung.h"
#include "eventloopwithstatus.h"
#include "dialogs/authenticationdialog.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationDomain(QStringLiteral("brunner.ninja"));
    QCoreApplication::setOrganizationName(QStringLiteral("db-software"));
    QCoreApplication::setApplicationName(QStringLiteral("zeiterfassung"));
    QCoreApplication::setApplicationVersion(QStringLiteral("1.0"));

    QSplashScreen splashScreen(QPixmap(":/zeiterfassung/images/splash.png"));
    splashScreen.showMessage(QObject::tr("Loading settings..."));
    splashScreen.show();

    QSettings settings;

    if(settings.value("url").isNull())
    {
        bool ok;
        auto url = QInputDialog::getText(&splashScreen, QObject::tr("Base url"),
                                         QObject::tr("Please enter the base url to the Zeiterfassung:"),
                                         QLineEdit::Normal, QString(), &ok);
        if(!ok)
            return -1;
        settings.setValue("url", url);
    }

    splashScreen.showMessage(QObject::tr("Loading login page..."));

    Zeiterfassung erfassung(settings.value("url").toString());

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
                                             QLineEdit::Normal, settings.value("url").toString(), &ok);
            if(!ok)
                return -1;
            settings.setValue("url", url);
            erfassung.setUrl(url);
            goto again1;
        }
    }

    splashScreen.showMessage(QObject::tr("Authenticating..."));

    if(settings.value("username").isNull() || settings.value("password").isNull())
    {
        AuthenticationDialog dialog(&splashScreen);
        if(dialog.exec() != QDialog::Accepted)
            return -1;
        settings.setValue("username", dialog.username());
        settings.setValue("password", dialog.password());
    }

    {
        EventLoopWithStatus eventLoop;
        QObject::connect(&erfassung, &Zeiterfassung::loginFinished, &eventLoop, &EventLoopWithStatus::quitWithStatus);

        again2:
        erfassung.doLogin(settings.value("username").toString(), settings.value("password").toString());
        eventLoop.exec();

        if(!eventLoop.success())
        {
            QMessageBox::warning(&splashScreen, QObject::tr("Could not authenticate with Zeiterfassung"),
                                 QObject::tr("The Zeiterfassung authentication was not successful:\n\n%0").arg(eventLoop.message()));

            AuthenticationDialog dialog(&splashScreen);
            dialog.setUsername(settings.value("username").toString());
            dialog.setPassword(settings.value("password").toString());
            if(dialog.exec() != QDialog::Accepted)
                return -1;
            settings.setValue("username", dialog.username());
            settings.setValue("password", dialog.password());

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
