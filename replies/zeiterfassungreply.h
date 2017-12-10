#ifndef ZEITERFASSUNGREPLY_H
#define ZEITERFASSUNGREPLY_H

#include <QObject>

class ZeiterfassungApi;

class ZeiterfassungReply : public QObject
{
    Q_OBJECT

public:
    explicit ZeiterfassungReply(ZeiterfassungApi *zeiterfassung);

protected:
    ZeiterfassungApi *zeiterfassung() const;

private:
    ZeiterfassungApi *m_zeiterfassung;
};

#endif // ZEITERFASSUNGREPLY_H
