#include "timeassignmentsmodel.h"

#include <QDebug>

#include "stripswidget.h"

TimeAssignmentsModel::TimeAssignmentsModel(QObject *parent) :
    QAbstractListModel(parent),
    m_stripsWidget(Q_NULLPTR),
    m_enabled(false)
{
}

StripsWidget *TimeAssignmentsModel::stripsWidget() const
{
    return m_stripsWidget;
}

void TimeAssignmentsModel::setStripsWidget(StripsWidget *stripsWidget)
{
    if(m_stripsWidget != stripsWidget)
    {
        if(m_stripsWidget)
        {
            disconnect(m_stripsWidget, &StripsWidget::timeAssignmentsChanged, this, &TimeAssignmentsModel::timeAssignmentsChanged);
            disconnect(m_stripsWidget, &StripsWidget::refreshingTimeAssignmentsChanged, this, &TimeAssignmentsModel::refreshingChanged);
        }

        beginResetModel();
        m_stripsWidget = stripsWidget;
        endResetModel();

        if(m_stripsWidget)
        {
            connect(m_stripsWidget, &StripsWidget::timeAssignmentsChanged, this, &TimeAssignmentsModel::timeAssignmentsChanged);
            connect(m_stripsWidget, &StripsWidget::refreshingTimeAssignmentsChanged, this, &TimeAssignmentsModel::refreshingChanged);

            if(m_enabled == m_stripsWidget->refreshingTimeAssignments())
                Q_EMIT enabledChanged(m_enabled = !m_stripsWidget->refreshingTimeAssignments());
        }
        else if(m_enabled)
            Q_EMIT enabledChanged(m_enabled = false);
    }
}

bool TimeAssignmentsModel::enabled() const
{
    return m_enabled;
}

int TimeAssignmentsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_stripsWidget ? m_stripsWidget->timeAssignments().count() : 0;
}

int TimeAssignmentsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 7;
}

QVariant TimeAssignmentsModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(m_stripsWidget != Q_NULLPTR);
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

void TimeAssignmentsModel::refreshingChanged(bool refreshing)
{
    if(m_enabled == refreshing)
        Q_EMIT enabledChanged(m_enabled = !refreshing);
}
