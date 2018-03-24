#include "zeiterfassungsettings.h"

const QString ZeiterfassungSettings::m_language("language");
const QString ZeiterfassungSettings::m_url("url");
const QString ZeiterfassungSettings::m_username("username");
const QString ZeiterfassungSettings::m_password("password");
const QString ZeiterfassungSettings::m_projects("projects");
const QString ZeiterfassungSettings::m_subprojects("subprojects");
const QString ZeiterfassungSettings::m_workpackages("workpackages");
const QString ZeiterfassungSettings::m_texts("texte");
const QString ZeiterfassungSettings::m_theme("theme");
const QLocale::Language ZeiterfassungSettings::m_defaultLanguage(QLocale::AnyLanguage);
const QUrl ZeiterfassungSettings::m_defaultUrl(QStringLiteral("http://10.1.0.11:8080/evoApps/"));

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
    return value(m_language, m_defaultLanguage).value<QLocale::Language>();
}

void ZeiterfassungSettings::setLanguage(QLocale::Language language)
{
    if(this->language() != language)
    {
        if(m_defaultLanguage == language)
            remove(m_language);
        else
            setValue(m_language, language);
        Q_EMIT languageChanged(language);
    }
}

QUrl ZeiterfassungSettings::url() const
{
    return value(m_url, m_defaultUrl).toUrl();
}

void ZeiterfassungSettings::setUrl(const QUrl &url)
{
    if(this->url() != url)
    {
        if(m_defaultUrl == url)
            remove(m_url);
        else
            setValue(m_url, url);
        Q_EMIT urlChanged(url);
    }
}

QString ZeiterfassungSettings::username() const
{
    return value(m_username).toString();
}

void ZeiterfassungSettings::setUsername(const QString &username)
{
    if(this->username() != username)
    {
        if(username.isEmpty())
            remove(m_username);
        else
            setValue(m_username, username);
        Q_EMIT usernameChanged(username);
    }
}

QString ZeiterfassungSettings::password() const
{
    return value(m_password).toString();
}

void ZeiterfassungSettings::setPassword(const QString &password)
{
    if(this->password() != password)
    {
        if(password.isEmpty())
            remove(m_password);
        else
            setValue(m_password, password);
        Q_EMIT passwordChanged(password);
    }
}

QStringList ZeiterfassungSettings::projects() const
{
    return value(m_projects).toStringList();
}

void ZeiterfassungSettings::setProjects(const QStringList &projects)
{
    if(this->projects() != projects)
    {
        if(projects.isEmpty())
            remove(m_projects);
        else
            setValue(m_projects, projects);
        Q_EMIT projectsChanged(projects);
    }
}

void ZeiterfassungSettings::prependProject(const QString &project)
{
    setProjects(prependItem(projects(), project));
}

QStringList ZeiterfassungSettings::subprojects() const
{
    return value(m_subprojects).toStringList();
}

void ZeiterfassungSettings::setSubprojects(const QStringList &subprojects)
{
    if(this->subprojects() != subprojects)
    {
        if(subprojects.isEmpty())
            remove(m_subprojects);
        else
            setValue(m_subprojects, subprojects);
        Q_EMIT subprojectsChanged(subprojects);
    }
}

void ZeiterfassungSettings::prependSubproject(const QString &subproject)
{
    setSubprojects(prependItem(subprojects(), subproject));
}

QStringList ZeiterfassungSettings::workpackages() const
{
    return value(m_workpackages).toStringList();
}

void ZeiterfassungSettings::setWorkpackages(const QStringList &workpackages)
{
    if(this->workpackages() != workpackages)
    {
        if(workpackages.isEmpty())
            remove(m_workpackages);
        else
            setValue(m_workpackages, workpackages);
        Q_EMIT workpackagesChanged(workpackages);
    }
}

void ZeiterfassungSettings::prependWorkpackage(const QString &workpackage)
{
    setWorkpackages(prependItem(workpackages(), workpackage));
}

QStringList ZeiterfassungSettings::texts() const
{
    return value(m_texts).toStringList();
}

void ZeiterfassungSettings::setTexts(const QStringList &texts)
{
    if(this->texts() != texts)
    {
        if(m_texts.isEmpty())
            remove(m_texts);
        else
            setValue(m_texts, texts);
        Q_EMIT textsChanged(texts);
    }
}

void ZeiterfassungSettings::prependText(const QString &text)
{
    setTexts(prependItem(texts(), text));
}

QString ZeiterfassungSettings::theme() const
{
    return value(m_theme).toString();
}

void ZeiterfassungSettings::setTheme(const QString &theme)
{
    if(this->theme() != theme)
    {
        if(theme.isEmpty())
            remove(m_theme);
        else
            setValue(m_theme, theme);
        Q_EMIT themeChanged(theme);
    }
}

QStringList ZeiterfassungSettings::prependItem(QStringList list, const QString &item)
{
    if(item.trimmed().isEmpty())
        return list;

    list.removeAll(item);
    list.prepend(item);

    return list;
}
