#pragma once

#include <memory>

#include <QString>
#include <QDate>
#include <QNetworkReply>

#include "zeiterfassungcorelib_global.h"
#include "zeiterfassungreply.h"

class ZeiterfassungApi;

class ZEITERFASSUNGCORELIBSHARED_EXPORT GetUserInfoReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    explicit GetUserInfoReply(std::unique_ptr<QNetworkReply> &&reply0, std::unique_ptr<QNetworkReply> &&reply1,
                     ZeiterfassungApi *zeiterfassung);

    struct UserInfo
    {
        int userId;
        QString email;
        QString longUsername;
        QString text;
        QString username;
        QString street;
        QString city;
        QDate employedSince;
        QDate employedTill;
        QString placeOfBirth;
        QString zipcode;
        QString religion;
        QString department;
        QString verwendgr;
        QString taetig;
        QString arbverh;
        QString betriebsnr;
    };

    const UserInfo &userInfo() const;

private Q_SLOTS:
    void request0Finished();
    void request1Finished();

private:
    std::unique_ptr<QNetworkReply> m_reply0;
    std::unique_ptr<QNetworkReply> m_reply1;
    UserInfo m_userInfo;
};
