#ifndef ZEITERFASSUNGSETTINGS_H
#define ZEITERFASSUNGSETTINGS_H

#include <QSettings>
#include <QLocale>

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

    QStringList projekte() const;
    void setProjekte(const QStringList &projekte);
    void prependProjekt(const QString &projekt);

    QStringList subprojekte() const;
    void setSubprojekte(const QStringList &subprojekte);
    void prependSubprojekt(const QString &subprojekt);

    QStringList workpackages() const;
    void setWorkpackages(const QStringList &workpackages);
    void prependWorkpackage(const QString &workpackage);

    QStringList texte() const;
    void setTexte(const QStringList &texte);
    void prependText(const QString &text);

    QString buchungKommenBackgroundColor() const;
    void setBuchungKommenBackgroundColor(const QString &buchungKommenBackgroundColor);

    QString buchungGehenBackgroundColor() const;
    void setBuchungGehenBackgroundColor(const QString &buchungGehenBackgroundColor);

    QString buchungOtherBackgroundColor() const;
    void setBuchungOtherBackgroundColor(const QString &buchungOtherBackgroundColor);

    QString timeAssignmentBackgroundColor() const;
    void setTimeAssignmentBackgroundColor(const QString timeAssignmentBackgroundColor);

private:
    void prepentItem(const QString &name, const QString &item);
};

#endif // ZEITERFASSUNGSETTINGS_H
