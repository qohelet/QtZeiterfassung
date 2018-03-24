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
    Q_PROPERTY(QLocale::Language language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QStringList projects READ projects WRITE setProjects NOTIFY projectsChanged)
    Q_PROPERTY(QStringList subprojects READ subprojects WRITE setSubprojects NOTIFY subprojectsChanged)
    Q_PROPERTY(QStringList workpackages READ workpackages WRITE setWorkpackages NOTIFY workpackagesChanged)
    Q_PROPERTY(QStringList texts READ texts WRITE setTexts NOTIFY textsChanged)
    Q_PROPERTY(QString theme READ theme WRITE setTheme NOTIFY themeChanged)

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

    QStringList texts() const;
    void setTexts(const QStringList &texts);
    void prependText(const QString &text);

    QString theme() const;
    void setTheme(const QString &theme);

Q_SIGNALS:
    void languageChanged(QLocale::Language language);
    void urlChanged(const QUrl &url);
    void usernameChanged(const QString &username);
    void passwordChanged(const QString &password);
    void projectsChanged(const QStringList &projects);
    void subprojectsChanged(const QStringList &subprojects);
    void workpackagesChanged(const QStringList &workpackages);
    void textsChanged(const QStringList &texts);
    void themeChanged(const QString &theme);

private:
    QStringList prependItem(QStringList list, const QString &item);

    static const QString m_language;
    static const QString m_url;
    static const QString m_username;
    static const QString m_password;
    static const QString m_projects;
    static const QString m_subprojects;
    static const QString m_workpackages;
    static const QString m_texts;
    static const QString m_theme;
    static const QLocale::Language m_defaultLanguage;
    static const QUrl m_defaultUrl;
};
