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

QStringList ZeiterfassungSettings::projekte() const
{
    return value("projekte").toStringList();
}

void ZeiterfassungSettings::setProjekte(const QStringList &projekte)
{
    setValue("projekte", projekte);
}

void ZeiterfassungSettings::prependProjekt(const QString &projekt)
{
    prepentItem("projekte", projekt);
}

QStringList ZeiterfassungSettings::subprojekte() const
{
    return value("subprojekte").toStringList();
}

void ZeiterfassungSettings::setSubprojekte(const QStringList &subprojekte)
{
    setValue("subprojekte", subprojekte);
}

void ZeiterfassungSettings::prependSubprojekt(const QString &subprojekt)
{
    prepentItem("subprojekte", subprojekt);
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

QString ZeiterfassungSettings::buchungStartBackgroundColor() const
{
    return value("buchungStartBackgroundColor", QStringLiteral("qlineargradient( x1:0 y1:0, x2:0 y2:1, stop:0 #7FFF7F, stop:1 #6FBF6F)")).toString();
}

void ZeiterfassungSettings::setBuchungStartBackgroundColor(const QString &buchungStartBackgroundColor)
{
    setValue("buchungStartBackgroundColor", buchungStartBackgroundColor);
}

QString ZeiterfassungSettings::buchungEndBackgroundColor() const
{
    return value("buchungEndBackgroundColor", QStringLiteral("qlineargradient( x1:0 y1:0, x2:0 y2:1, stop:0 #FF7F7F, stop:1 #BF6F6F)")).toString();
}

void ZeiterfassungSettings::setBuchungEndBackgroundColor(const QString &buchungEndBackgroundColor)
{
    setValue("buchungEndBackgroundColor", buchungEndBackgroundColor);
}

QString ZeiterfassungSettings::buchungOtherBackgroundColor() const
{
    return value("buchungOtherBackgroundColor", QStringLiteral("qlineargradient( x1:0 y1:0, x2:0 y2:1, stop:0 #FFFF7F, stop:1 #BFBF6F)")).toString();
}

void ZeiterfassungSettings::setBuchungOtherBackgroundColor(const QString &buchungOtherBackgroundColor)
{
    setValue("buchungOtherBackgroundColor", buchungOtherBackgroundColor);
}

QString ZeiterfassungSettings::timeAssignmentBackgroundColor() const
{
    return value("timeAssignmentBackgroundColor", QStringLiteral("qlineargradient( x1:0 y1:0, x2:0 y2:1, stop:0 #7FFFFF, stop:1 #6FBFBF)")).toString();
}

void ZeiterfassungSettings::setTimeAssignmentBackgroundColor(const QString timeAssignmentBackgroundColor)
{
    setValue("timeAssignmentBackgroundColor", timeAssignmentBackgroundColor);
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
