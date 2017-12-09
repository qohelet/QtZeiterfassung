#ifndef ZEITERFASSUNGSETTINGS_H
#define ZEITERFASSUNGSETTINGS_H

#include <QSettings>
#include <QLocale>
#include <QString>
#include <QStringList>
#include <QDate>

class ZeiterfassungSettings : public QSettings
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

    QString url() const;
    void setUrl(const QString &url);

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

    QDate lastUpdateCheck() const;
    void setLastUpdateCheck(const QDate &lastUpdateCheck);

private:
    void prepentItem(const QString &name, const QString &item);
};

#endif // ZEITERFASSUNGSETTINGS_H
