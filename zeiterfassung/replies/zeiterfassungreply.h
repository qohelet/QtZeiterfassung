#ifndef ZEITERFASSUNGREPLY_H
#define ZEITERFASSUNGREPLY_H

#include <QObject>

class ZeiterfassungApi;

class ZeiterfassungReply : public QObject
{
    Q_OBJECT

public:
    explicit ZeiterfassungReply(ZeiterfassungApi *zeiterfassung);

    bool success() const;
    const QString &message() const;

Q_SIGNALS:
    void finished();

protected:
    ZeiterfassungApi *zeiterfassung() const;
    void setSuccess(bool success);
    void setMessage(const QString &message);

private:
    ZeiterfassungApi *m_zeiterfassung;
    bool m_success;
    QString m_message;
};

#endif // ZEITERFASSUNGREPLY_H
