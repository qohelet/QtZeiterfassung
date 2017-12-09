#ifndef TIMEASSIGNMENTSMODEL_H
#define TIMEASSIGNMENTSMODEL_H

#include <QAbstractListModel>
#include <QVector>

#include "zeiterfassung.h"

class TimeAssignmentsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TimeAssignmentsModel(Zeiterfassung &erfassung, QObject *parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;

    Zeiterfassung::TimeAssignment getTimeAssignment(const QModelIndex &index) const;

    bool refresh(int userId, const QDate &from, const QDate &to);

    const QVector<Zeiterfassung::TimeAssignment> timeAssignments() const;

Q_SIGNALS:
    void refreshFinished(bool success, const QString &message);

private Q_SLOTS:
    void getTimeAssignmentsFinished(bool success, const QString &message, const QVector<Zeiterfassung::TimeAssignment> &timeAssignments);

private:
    Zeiterfassung &m_erfassung;
    QVector<Zeiterfassung::TimeAssignment> m_timeAssignments;
};

#endif // TIMEASSIGNMENTSMODEL_H
