#ifndef ZEITERFASSUNG_H
#define ZEITERFASSUNG_H

#include <QObject>
#include <QString>
#include <QDate>
#include <QTime>

class QNetworkAccessManager;
class QNetworkReply;
class QByteArray;

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

    struct TimeAssignment
    {
        int id;
        QDate date;
        QTime time;
        QTime timespan;
        QString text;
        QString project;
        QString subproject;
        QString workpackage;
    };

    struct Project
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

    bool doGetTimeAssignments(int userId, const QDate &start, const QDate &end);
    bool doCreateTimeAssignment(int userId, const QDate &date, const QTime &time, const QTime &timespan,
                                const QString &project, const QString &subproject, const QString &workpackage,
                                const QString &text);
    bool doUpdateTimeAssignment(int timeAssignmentId, int userId, const QDate &date, const QTime &time,
                                const QTime &timespan, const QString &project, const QString &subproject,
                                const QString &workpackage, const QString &text);
    bool doDeleteTimeAssignment(int timeAssignmentId);

    bool doGetProjects(int userId, const QDate &date);
    bool doGetAuswertung(int userId, const QDate &date);

Q_SIGNALS:
    void urlChanged(const QString &url);

    void loginPageFinished(bool success, const QString &message);
    void loginFinished(bool success, const QString &message);
    void userInfoFinished(bool success, const QString &message, const UserInfo &userInfo);

    void getBuchungenFinished(bool success, const QString &message, const QVector<Buchung> &buchungen);
    void createBuchungFinished(bool success, const QString &message, int buchungId);
    void updateBuchungFinished(bool success, const QString &message, int buchungId);
    void deleteBuchungFinished(bool success, const QString &message);

    void getTimeAssignmentsFinished(bool success, const QString &message, const QVector<TimeAssignment> &timeAssignments);
    void createTimeAssignmentFinished(bool success, const QString &message, int timeAssignmentId);
    void updateTimeAssignmentFinished(bool success, const QString &message, int timeAssignmentId);
    void deleteTimeAssignmentFinished(bool success, const QString &message);

    void getProjectsFinished(bool success, const QString &message, const QVector<Project> &projects);
    void getAuswertungFinished(bool success, const QString &message, const QByteArray &content);

private Q_SLOTS:
    void loginPageRequestFinished();
    void loginRequestFinished();
    void userInfoRequestFinished();

    void getBuchungenRequestFinished();
    void createBuchungRequestFinished();
    void updateBuchungRequestFinished();
    void deleteBuchungRequestFinished();

    void getTimeAssignmentsRequestFinished();
    void createTimeAssignmentRequestFinished();
    void updateTimeAssignmentRequestFinished();
    void deleteTimeAssignmentRequestFinished();

    void getProjectsRequestFinished();
    void getAuswertungRequest0Finished();
    void getAuswertungRequest1Finished();

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

        QNetworkReply *getTimeAssignments;
        QNetworkReply *createTimeAssignment;
        QNetworkReply *updateTimeAssignment;
        QNetworkReply *deleteTimeAssignment;

        QNetworkReply *getProjects;
        QNetworkReply *getAuswertung;
    } m_replies;
};

#endif // ZEITERFASSUNG_H
