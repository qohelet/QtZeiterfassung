#ifndef LOGMODEL_H
#define LOGMODEL_H

#include <QAbstractListModel>
#include <QDateTime>
#include <qlogging.h>
#include <QList>

class LogModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit LogModel(QObject *parent = Q_NULLPTR);

    void log(QtMsgType type, const char *fileName, int lineNumber, const char *functionName, const char *categoryName, const QString &message);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;

private:
    struct Entry
    {
        QDateTime dateTime;
        QtMsgType type;
        const char *fileName;
        int lineNumber;
        const char *functionName;
        const char *categoryName;
        QString message;
    };

    QList<Entry> m_entries;
};

#endif // LOGMODEL_H
