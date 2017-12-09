#include "timeassignmentsmodel.h"

TimeAssignmentsModel::TimeAssignmentsModel(Zeiterfassung &erfassung, QObject *parent) :
    QAbstractListModel(parent),
    m_erfassung(erfassung)
{

}

int TimeAssignmentsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_timeAssignments.count();
}

int TimeAssignmentsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 7;
}

QVariant TimeAssignmentsModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.row() < m_timeAssignments.count());
    const auto &timeAssignment = m_timeAssignments.at(index.row());

    switch(role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        switch(index.column())
        {
        case 0: return timeAssignment.id;
        case 1: return timeAssignment.time;
        case 2: return timeAssignment.timespan;
        case 3: return timeAssignment.project;
        case 4: return timeAssignment.subproject;
        case 5: return timeAssignment.workpackage;
        case 6: return timeAssignment.text;
        }
    }

    return QVariant();
}

QVariant TimeAssignmentsModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            case 3: return tr("Project");
            case 4: return tr("Subproject");
            case 5: return tr("Workpackage");
            case 6: return tr("Text");
            }
        }
    }

    return QVariant();
}

Zeiterfassung::TimeAssignment TimeAssignmentsModel::getTimeAssignment(const QModelIndex &index) const
{
    if(!index.isValid())
        return Zeiterfassung::TimeAssignment();

    Q_ASSERT(index.row() <= m_timeAssignments.count());
    return m_timeAssignments.at(index.row());
}

bool TimeAssignmentsModel::refresh(int userId, const QDate &from, const QDate &to)
{
    if(!m_erfassung.doGetTimeAssignments(userId, from, to))
        return false;

    beginResetModel();
    m_timeAssignments.clear();
    endResetModel();

    connect(&m_erfassung, &Zeiterfassung::getTimeAssignmentsFinished,
            this,         &TimeAssignmentsModel::getTimeAssignmentsFinished);

    return true;
}

const QVector<Zeiterfassung::TimeAssignment> TimeAssignmentsModel::timeAssignments() const
{
    return m_timeAssignments;
}

void TimeAssignmentsModel::getTimeAssignmentsFinished(bool success, const QString &message, const QVector<Zeiterfassung::TimeAssignment> &timeAssignments)
{
    disconnect(&m_erfassung, &Zeiterfassung::getTimeAssignmentsFinished,
               this,         &TimeAssignmentsModel::getTimeAssignmentsFinished);

    if(success)
    {
        beginResetModel();
        m_timeAssignments = timeAssignments;
        endResetModel();
    }

    Q_EMIT refreshFinished(success, message);
}
