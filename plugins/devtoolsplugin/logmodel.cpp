#include "logmodel.h"

LogModel::LogModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

void LogModel::log(QtMsgType type, const char *fileName, int lineNumber, const char *functionName, const char *categoryName, const QString &message)
{
    beginInsertRows(QModelIndex(), m_entries.count(), m_entries.count());
    m_entries.append(Entry { QDateTime::currentDateTime(), type, fileName, lineNumber, functionName, categoryName, message });
    endInsertRows();
}

int LogModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_entries.count();
}

int LogModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 4;
}

QVariant LogModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.row() < m_entries.count());
    const auto &entry = m_entries.at(index.row());

    switch(role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        switch(index.column())
        {
        case 0:
        {
            switch(entry.type)
            {
            case QtDebugMsg: return tr("Debug");
            case QtWarningMsg: return tr("Warning");
            case QtCriticalMsg: return tr("Critical");
            case QtFatalMsg: return tr("Fatal");
            case QtInfoMsg: return tr("Info");
            }
        }
        case 1: return entry.dateTime.toString(QStringLiteral("dd.MM.yyyy HH:mm:ss.zzz"));
        case 2: return entry.functionName;
        case 3: return entry.message;
        }
    }

    return QVariant();
}

QVariant LogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch(orientation)
    {
    case Qt::Horizontal:
        switch(role)
        {
        case Qt::DisplayRole:
        case Qt::EditRole:
            switch(section)
            {
            case 0: return tr("Type");
            case 1: return tr("Timestamp");
            case 2: return tr("Function");
            case 3: return tr("Message");
            }
        }
    default:
        qt_noop();
    }

    return QVariant();
}
