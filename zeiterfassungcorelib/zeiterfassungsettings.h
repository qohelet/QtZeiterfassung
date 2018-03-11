#pragma once

#include <QSettings>
#include <QLocale>
#include <QUrl>
#include <QString>
#include <QStringList>
#include <QDate>

#include "zeiterfassungcorelib_global.h"

class ZEITERFASSUNGCORELIBSHARED_EXPORT ZeiterfassungSettings : public QSettings
{
    Q_OBJECT

public:
    explicit ZeiterfassungSettings(const QString &organization,
                                   const QString &application = QString(), QObject *parent = Q_NULLPTR);
    ZeiterfassungSettings(Scope scope, const QString &organization,
                          const QString &application = QString(), QObject *parent = Q_NULLPTR);
    ZeiterfassungSettings(Format format, Scope scope, const QString &organization,
                          const QString &application = QString(), QObject *parent = Q_NULLPTR);
    ZeiterfassungSettings(const QString &fileName, Format format, QObject *parent = Q_NULLPTR);
    explicit ZeiterfassungSettings(QObject *parent = Q_NULLPTR);

    QLocale::Language language() const;
    void setLanguage(QLocale::Language language);

    QUrl url() const;
    void setUrl(const QUrl &url);

    QString username() const;
    void setUsername(const QString &username);

    QString password() const;
    void setPassword(const QString &password);

    QStringList projects() const;
    void setProjects(const QStringList &projects);
    void prependProject(const QString &project);

    QStringList subprojects() const;
    void setSubprojects(const QStringList &subprojects);
    void prependSubproject(const QString &subproject);

    QStringList workpackages() const;
    void setWorkpackages(const QStringList &workpackages);
    void prependWorkpackage(const QString &workpackage);

    QStringList texte() const;
    void setTexte(const QStringList &texte);
    void prependText(const QString &text);

    QString theme() const;
    void setTheme(const QString &theme);

private:
    void prependItem(const QString &name, const QString &item);
};
