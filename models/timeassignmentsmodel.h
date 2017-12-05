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

    typedef QVector<Zeiterfassung::TimeAssignment>::iterator iterator;
    typedef QVector<Zeiterfassung::TimeAssignment>::const_iterator const_iterator;
    typedef QVector<Zeiterfassung::TimeAssignment>::reverse_iterator reverse_iterator;
    typedef QVector<Zeiterfassung::TimeAssignment>::const_reverse_iterator const_reverse_iterator;

#if !defined(QT_STRICT_ITERATORS)
    //inline iterator begin() { return m_timeAssignments.begin(); }
    inline const_iterator begin() const Q_DECL_NOTHROW { return m_timeAssignments.begin(); }
    inline const_iterator cbegin() const Q_DECL_NOTHROW { return m_timeAssignments.cbegin(); }
    inline const_iterator constBegin() const Q_DECL_NOTHROW { return m_timeAssignments.constBegin(); }
    //inline iterator end() { return m_timeAssignments.end(); }
    inline const_iterator end() const Q_DECL_NOTHROW { return m_timeAssignments.end(); }
    inline const_iterator cend() const Q_DECL_NOTHROW { return m_timeAssignments.cend(); }
    inline const_iterator constEnd() const Q_DECL_NOTHROW { return m_timeAssignments.constEnd(); }
#else
    //inline iterator begin(iterator = iterator()) { return m_timeAssignments.begin(); }
    inline const_iterator begin(const_iterator = const_iterator()) const Q_DECL_NOTHROW { return m_timeAssignments.begin(); }
    inline const_iterator cbegin(const_iterator = const_iterator()) const Q_DECL_NOTHROW { return d->constBegin(); }
    inline const_iterator constBegin(const_iterator = const_iterator()) const Q_DECL_NOTHROW { return m_timeAssignments.constBegin(); }
    //inline iterator end(iterator = iterator()) { return m_timeAssignments.end(); }
    inline const_iterator end(const_iterator = const_iterator()) const Q_DECL_NOTHROW { return m_timeAssignments.end(); }
    inline const_iterator cend(const_iterator = const_iterator()) const Q_DECL_NOTHROW { return m_timeAssignments.cend(); }
    inline const_iterator constEnd(const_iterator = const_iterator()) const Q_DECL_NOTHROW { return m_timeAssignments.constEnd(); }
#endif
    //reverse_iterator rbegin() { return m_timeAssignments.rbegin(); }
    //reverse_iterator rend() { return m_timeAssignments.rbegin(); }
    const_reverse_iterator rbegin() const Q_DECL_NOTHROW { return m_timeAssignments.rbegin(); }
    const_reverse_iterator rend() const Q_DECL_NOTHROW { return m_timeAssignments.rend(); }
    const_reverse_iterator crbegin() const Q_DECL_NOTHROW { return m_timeAssignments.crbegin(); }
    const_reverse_iterator crend() const Q_DECL_NOTHROW { return m_timeAssignments.crend(); }

Q_SIGNALS:
    void refreshFinished(bool success, const QString &message);

private Q_SLOTS:
    void getTimeAssignmentsFinished(bool success, const QString &message, const QVector<Zeiterfassung::TimeAssignment> &timeAssignments);

private:
    Zeiterfassung &m_erfassung;
    QVector<Zeiterfassung::TimeAssignment> m_timeAssignments;
};

#endif // TIMEASSIGNMENTSMODEL_H
