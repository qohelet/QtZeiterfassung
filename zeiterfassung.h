#ifndef ZEITERFASSUNG_H
#define ZEITERFASSUNG_H

#include <QObject>
#include <QString>
#include <QDate>
#include <QTime>

class QNetworkAccessManager;
class QNetworkReply;

class Zeiterfassung : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)

public:
    explicit Zeiterfassung(const QString &url, QObject *parent = 0);

    const QString &url() const;
    void setUrl(const QString &url);

    struct UserInfo
    {
        int userId;
        QString email;
        QString longUsername;
        QString text;
        QString username;
    };

    struct Buchung
    {
        int id;
        QDate date;
        QTime time;
        QTime timespan;
        QString type;
        QString text;
    };

    struct Kontierung
    {
        int id;
        QDate date;
        QTime time;
        QTime timespan;
        QString text;
        QString projekt;
        QString subprojekt;
        QString workpackage;
    };

    struct Projekt
    {
        QString label;
        QString value;
    };

public Q_SLOTS:
    bool doLoginPage();
    bool doLogin(const QString &username, const QString &password);
    bool doUserInfo();

    bool doGetBuchungen(int userId, const QDate &start, const QDate &end);
    bool doCreateBuchung(int userId, const QDate &date, const QTime &time, const QTime &timespan,
                         const QString &type, const QString &text);
    bool doUpdateBuchung(int buchungId, int userId, const QDate &date, const QTime &time,
                         const QTime &timespan, const QString &type, const QString &text);
    bool doDeleteBuchung(int buchungId);

    bool doGetKontierungen(int userId, const QDate &start, const QDate &end);
    bool doCreateKontierung(int userId, const QDate &date, const QTime &time, const QTime &timespan,
                            const QString &projekt, const QString &subprojekt, const QString &workpackage,
                            const QString &text);
    bool doUpdateKontierung(int kontierungId, int userId, const QDate &date, const QTime &time,
                            const QTime &timespan, const QString &projekt, const QString &subprojekt,
                            const QString &workpackage, const QString &text);
    bool doDeleteKontierung(int kontierungId);

    bool doGetProjekte(int userId, QDate date = QDate::currentDate());

Q_SIGNALS:
    void urlChanged(const QString &url);

    void loginPageFinished(bool success, const QString &message);
    void loginFinished(bool success, const QString &message);
    void userInfoFinished(bool success, const QString &message, const UserInfo &userInfo);

    void getBuchungenFinished(bool success, const QString &message, const QVector<Buchung> &buchungen);
    void createBuchungFinished(bool success, const QString &message, int buchungId);
    void updateBuchungFinished(bool success, const QString &message, int buchungId);
    void deleteBuchungFinished(bool success, const QString &message);

    void getKontierungenFinished(bool success, const QString &message, const QVector<Kontierung> &kontierungen);
    void createKontierungFinished(bool success, const QString &message, int buchungId);
    void updateKontierungFinished(bool success, const QString &message, int buchungId);
    void deleteKontierungFinished(bool success, const QString &message);

    void getProjekteFinished(bool success, const QString &message, const QVector<Projekt> &projekte);

private Q_SLOTS:
    void loginPageRequestFinished();
    void loginRequestFinished();
    void userInfoRequestFinished();

    void getBuchungenRequestFinished();
    void createBuchungRequestFinished();
    void updateBuchungRequestFinished();
    void deleteBuchungRequestFinished();

    void getKontierungenRequestFinished();
    void createKontierungRequestFinished();
    void updateKontierungRequestFinished();
    void deleteKontierungRequestFinished();

    void getProjekteRequestFinished();

private:
    QString m_url;
    QNetworkAccessManager *m_manager;

    struct {
        QNetworkReply *loginPage;
        QNetworkReply *login;
        QNetworkReply *userInfo;

        QNetworkReply *getBuchungen;
        QNetworkReply *createBuchung;
        QNetworkReply *updateBuchung;
        QNetworkReply *deleteBuchung;

        QNetworkReply *getKontierungen;
        QNetworkReply *createKontierung;
        QNetworkReply *updateKontierung;
        QNetworkReply *deleteKontierung;

        QNetworkReply *getProjekte;
    } m_replies;
};

#endif // ZEITERFASSUNG_H
