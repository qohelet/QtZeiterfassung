#ifndef TIMEASSIGNMENTSMODEL_H
#define TIMEASSIGNMENTSMODEL_H

#include <QAbstractListModel>
#include <QVector>

#include "zeiterfassungapi.h"

class TimeAssignmentsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TimeAssignmentsModel(ZeiterfassungApi &erfassung, QObject *parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;

    ZeiterfassungApi::TimeAssignment getTimeAssignment(const QModelIndex &index) const;

    bool refresh(int userId, const QDate &from, const QDate &to);

    const QVector<ZeiterfassungApi::TimeAssignment> timeAssignments() const;

Q_SIGNALS:
    void refreshFinished(bool success, const QString &message);

private Q_SLOTS:
    void getTimeAssignmentsFinished(bool success, const QString &message, const QVector<ZeiterfassungApi::TimeAssignment> &timeAssignments);

private:
    ZeiterfassungApi &m_erfassung;
    QVector<ZeiterfassungApi::TimeAssignment> m_timeAssignments;
};

#endif // TIMEASSIGNMENTSMODEL_H
