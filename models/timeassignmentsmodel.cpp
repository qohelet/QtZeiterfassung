#include "timeassignmentsmodel.h"

#include "stripswidget.h"

TimeAssignmentsModel::TimeAssignmentsModel(StripsWidget *stripsWidget) :
    QAbstractListModel(stripsWidget),
    m_stripsWidget(stripsWidget)
{
    connect(stripsWidget, &StripsWidget::timeAssignmentsChanged, this, &TimeAssignmentsModel::timeAssignmentsChanged);
}

int TimeAssignmentsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_stripsWidget->timeAssignments().count();
}

int TimeAssignmentsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 7;
}

QVariant TimeAssignmentsModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.row() < m_stripsWidget->timeAssignments().count());
    const auto &timeAssignment = m_stripsWidget->timeAssignments().at(index.row());

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

void TimeAssignmentsModel::timeAssignmentsChanged()
{
    beginResetModel();
    endResetModel();
}
