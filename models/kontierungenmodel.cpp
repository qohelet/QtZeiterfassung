#include "kontierungenmodel.h"

KontierungenModel::KontierungenModel(Zeiterfassung &erfassung, QObject *parent) :
    QAbstractListModel(parent),
    m_erfassung(erfassung)
{

}

int KontierungenModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_kontierungen.count();
}

int KontierungenModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 7;
}

QVariant KontierungenModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.row() < m_kontierungen.count());
    const auto &kontierung = m_kontierungen.at(index.row());

    switch(role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        switch(index.column())
        {
        case 0: return kontierung.id;
        case 1: return kontierung.time;
        case 2: return kontierung.timespan;
        case 3: return kontierung.projekt;
        case 4: return kontierung.subprojekt;
        case 5: return kontierung.workpackage;
        case 6: return kontierung.text;
        }
    }

    return QVariant();
}

QVariant KontierungenModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            case 3: return tr("Projekt");
            case 4: return tr("Subprojekt");
            case 5: return tr("Workpackage");
            case 6: return tr("Text");
            }
        }
    }

    return QVariant();
}

Zeiterfassung::Kontierung KontierungenModel::getKontierung(const QModelIndex &index) const
{
    if(!index.isValid())
        return Zeiterfassung::Kontierung();

    Q_ASSERT(index.row() <= m_kontierungen.count());
    return m_kontierungen.at(index.row());
}

bool KontierungenModel::refresh(int userId, const QDate &from, const QDate &to)
{
    if(!m_erfassung.doGetKontierungen(userId, from, to))
        return false;

    beginResetModel();
    m_kontierungen.clear();
    endResetModel();

    connect(&m_erfassung, &Zeiterfassung::getKontierungenFinished,
            this,         &KontierungenModel::getKontierungenFinished);

    return true;
}

void KontierungenModel::getKontierungenFinished(bool success, const QString &message, const QVector<Zeiterfassung::Kontierung> &kontierungen)
{
    disconnect(&m_erfassung, &Zeiterfassung::getKontierungenFinished,
               this,         &KontierungenModel::getKontierungenFinished);

    if(success)
    {
        beginResetModel();
        m_kontierungen = kontierungen;
        endResetModel();
    }

    Q_EMIT refreshFinished(success, message);
}
