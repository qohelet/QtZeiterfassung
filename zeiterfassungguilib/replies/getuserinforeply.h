#pragma once

#include <memory>

#include <QNetworkReply>

#include "zeiterfassungguilib_global.h"
#include "zeiterfassungreply.h"

class ZeiterfassungApi;

class ZEITERFASSUNGGUILIBSHARED_EXPORT GetUserInfoReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    GetUserInfoReply(std::unique_ptr<QNetworkReply> &&reply0, std::unique_ptr<QNetworkReply> &&reply1,
                     ZeiterfassungApi *zeiterfassung);

    struct UserInfo
    {
        int userId;
        QString email;
        QString longUsername;
        QString text;
        QString username;
        QString gemeinde;
        QString ort;
        QDate angFrom;
        QDate angTill;
        QString gebOrt;
        QString plz;
        QString religion;
        QString bereich;
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
