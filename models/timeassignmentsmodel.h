#ifndef TIMEASSIGNMENTSMODEL_H
#define TIMEASSIGNMENTSMODEL_H

#include <QAbstractListModel>
#include <QVector>

#include "zeiterfassungapi.h"

class StripsWidget;

class TimeAssignmentsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TimeAssignmentsModel(StripsWidget *stripsWidget);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;

private Q_SLOTS:
    void timeAssignmentsChanged();

private:
    StripsWidget *m_stripsWidget;
};

#endif // TIMEASSIGNMENTSMODEL_H
