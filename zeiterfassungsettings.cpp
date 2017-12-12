#include "zeiterfassungsettings.h"

ZeiterfassungSettings::ZeiterfassungSettings(const QString &organization,
                                             const QString &application, QObject *parent) :
    QSettings(organization, application, parent)
{

}

ZeiterfassungSettings::ZeiterfassungSettings(QSettings::Scope scope, const QString &organization,
                                             const QString &application, QObject *parent) :
    QSettings(scope, organization, application, parent)
{

}

ZeiterfassungSettings::ZeiterfassungSettings(QSettings::Format format, QSettings::Scope scope, const QString &organization, const QString &application, QObject *parent) :
    QSettings(format, scope, organization, application, parent)
{

}

ZeiterfassungSettings::ZeiterfassungSettings(const QString &fileName, QSettings::Format format, QObject *parent) :
    QSettings(fileName, format, parent)
{

}

ZeiterfassungSettings::ZeiterfassungSettings(QObject *parent) :
    QSettings(parent)
{

}

QLocale::Language ZeiterfassungSettings::language() const
{
    return value(QStringLiteral("language"), QLocale::AnyLanguage).value<QLocale::Language>();
}

void ZeiterfassungSettings::setLanguage(QLocale::Language language)
{
    setValue(QStringLiteral("language"), language);
}

QString ZeiterfassungSettings::url() const
{
    return value(QStringLiteral("url"), QStringLiteral("http://10.1.0.11:8080/evoApps/")).toString();
}

void ZeiterfassungSettings::setUrl(const QString &url)
{
    setValue(QStringLiteral("url"), url);
}

QString ZeiterfassungSettings::username() const
{
    return value(QStringLiteral("username")).toString();
}

void ZeiterfassungSettings::setUsername(const QString &username)
{
    setValue(QStringLiteral("username"), username);
}

QString ZeiterfassungSettings::password() const
{
    return value(QStringLiteral("password")).toString();
}

void ZeiterfassungSettings::setPassword(const QString &password)
{
    setValue(QStringLiteral("password"), password);
}

QStringList ZeiterfassungSettings::projects() const
{
    return value(QStringLiteral("projects")).toStringList();
}

void ZeiterfassungSettings::setProjects(const QStringList &projects)
{
    setValue(QStringLiteral("projects"), projects);
}

void ZeiterfassungSettings::prependProject(const QString &project)
{
    prependItem(QStringLiteral("projects"), project);
}

QStringList ZeiterfassungSettings::subprojects() const
{
    return value(QStringLiteral("subprojects")).toStringList();
}

void ZeiterfassungSettings::setSubprojects(const QStringList &subprojects)
{
    setValue(QStringLiteral("subprojects"), subprojects);
}

void ZeiterfassungSettings::prependSubproject(const QString &subproject)
{
    prependItem(QStringLiteral("subprojects"), subproject);
}

QStringList ZeiterfassungSettings::workpackages() const
{
    return value(QStringLiteral("workpackages")).toStringList();
}

void ZeiterfassungSettings::setWorkpackages(const QStringList &workpackages)
{
    setValue(QStringLiteral("workpackages"), workpackages);
}

void ZeiterfassungSettings::prependWorkpackage(const QString &workpackage)
{
    prependItem(QStringLiteral("workpackages"), workpackage);
}

QStringList ZeiterfassungSettings::texte() const
{
    return value(QStringLiteral("texte")).toStringList();
}

void ZeiterfassungSettings::setTexte(const QStringList &texte)
{
    setValue(QStringLiteral("texte"), texte);
}

void ZeiterfassungSettings::prependText(const QString &text)
{
    prependItem(QStringLiteral("texte"), text);
}

QDate ZeiterfassungSettings::lastUpdateCheck() const
{
    return value(QStringLiteral("lastUpdateCheck")).toDate();
}

void ZeiterfassungSettings::setLastUpdateCheck(const QDate &lastUpdateCheck)
{
    setValue(QStringLiteral("lastUpdateCheck"), lastUpdateCheck);
}

QString ZeiterfassungSettings::theme() const
{
    return value(QStringLiteral("theme")).toString();
}

void ZeiterfassungSettings::setTheme(const QString &theme)
{
    setValue(QStringLiteral("theme"), theme);
}

void ZeiterfassungSettings::prependItem(const QString &name, const QString &item)
{
    if(item.trimmed().isEmpty())
        return;

    auto entries = value(name).toStringList();
    entries.removeAll(item);
    entries.prepend(item);
    setValue(name, entries);
}
