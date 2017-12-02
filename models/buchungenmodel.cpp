#include "buchungenmodel.h"

BuchungenModel::BuchungenModel(Zeiterfassung &erfassung, QObject *parent) :
    QAbstractListModel(parent),
    m_erfassung(erfassung)
{

}

int BuchungenModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_buchungen.count();
}

int BuchungenModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 5;
}

QVariant BuchungenModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.row() < m_buchungen.count());
    const auto &buchung = m_buchungen.at(index.row());

    switch(role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        switch(index.column())
        {
        case 0: return buchung.id;
        case 1: return buchung.time;
        case 2: return buchung.timespan;
        case 3: return buchung.type;
        case 4: return buchung.text;
        }
    }

    return QVariant();
}

QVariant BuchungenModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            case 0: return tr("ID");
            case 1: return tr("Time");
            case 2: return tr("Timespan");
            case 3: return tr("Type");
            case 4: return tr("Text");
            }
        }
    }

    return QVariant();
}

Zeiterfassung::Buchung BuchungenModel::getBuchung(const QModelIndex &index) const
{
    if(!index.isValid())
        return Zeiterfassung::Buchung();

    Q_ASSERT(index.row() <= m_buchungen.count());
    return m_buchungen.at(index.row());
}

bool BuchungenModel::refresh(int userId, const QDate &from, const QDate &to)
{
    if(!m_erfassung.doGetBuchungen(userId, from, to))
        return false;

    beginResetModel();
    m_buchungen.clear();
    endResetModel();

    connect(&m_erfassung, &Zeiterfassung::getBuchungenFinished,
            this,         &BuchungenModel::getBuchungenFinished);

    return true;
}

void BuchungenModel::getBuchungenFinished(bool success, const QString &message, const QVector<Zeiterfassung::Buchung> &buchungen)
{
    disconnect(&m_erfassung, &Zeiterfassung::getBuchungenFinished,
               this,         &BuchungenModel::getBuchungenFinished);

    if(success)
    {
        beginResetModel();
        m_buchungen = buchungen;
        endResetModel();
    }

    Q_EMIT refreshFinished(success, message);
}
