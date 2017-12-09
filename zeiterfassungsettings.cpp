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
    return value("language").value<QLocale::Language>();
}

void ZeiterfassungSettings::setLanguage(QLocale::Language language)
{
    setValue("language", language);
}

QString ZeiterfassungSettings::url() const
{
    return value("url", QStringLiteral("http://10.1.0.11:8080/evoApps/")).toString();
}

void ZeiterfassungSettings::setUrl(const QString &url)
{
    setValue("url", url);
}

QString ZeiterfassungSettings::username() const
{
    return value("username").toString();
}

void ZeiterfassungSettings::setUsername(const QString &username)
{
    setValue("username", username);
}

QString ZeiterfassungSettings::password() const
{
    return value("password").toString();
}

void ZeiterfassungSettings::setPassword(const QString &password)
{
    setValue("password", password);
}

QStringList ZeiterfassungSettings::projects() const
{
    return value("projects").toStringList();
}

void ZeiterfassungSettings::setProjects(const QStringList &projects)
{
    setValue("projects", projects);
}

void ZeiterfassungSettings::prependProject(const QString &project)
{
    prepentItem("projects", project);
}

QStringList ZeiterfassungSettings::subprojects() const
{
    return value("subprojects").toStringList();
}

void ZeiterfassungSettings::setSubprojects(const QStringList &subprojects)
{
    setValue("subprojects", subprojects);
}

void ZeiterfassungSettings::prependSubproject(const QString &subproject)
{
    prepentItem("subprojects", subproject);
}

QStringList ZeiterfassungSettings::workpackages() const
{
    return value("workpackages").toStringList();
}

void ZeiterfassungSettings::setWorkpackages(const QStringList &workpackages)
{
    setValue("workpackages", workpackages);
}

void ZeiterfassungSettings::prependWorkpackage(const QString &workpackage)
{
    prepentItem("workpackages", workpackage);
}

QStringList ZeiterfassungSettings::texte() const
{
    return value("texte").toStringList();
}

void ZeiterfassungSettings::setTexte(const QStringList &texte)
{
    setValue("texte", texte);
}

void ZeiterfassungSettings::prependText(const QString &text)
{
    prepentItem("texte", text);
}

QDate ZeiterfassungSettings::lastUpdateCheck() const
{
    return value("lastUpdateCheck").toDate();
}

void ZeiterfassungSettings::setLastUpdateCheck(const QDate &lastUpdateCheck)
{
    setValue("lastUpdateCheck", lastUpdateCheck);
}

void ZeiterfassungSettings::prepentItem(const QString &name, const QString &item)
{
    if(item.trimmed().isEmpty())
        return;

    auto entries = value(name).toStringList();
    entries.removeAll(item);
    entries.prepend(item);
    setValue(name, entries);
}
