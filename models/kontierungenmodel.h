#ifndef KONTIERUNGENMODEL_H
#define KONTIERUNGENMODEL_H

#include <QAbstractListModel>
#include <QVector>

#include "zeiterfassung.h"

class KontierungenModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit KontierungenModel(Zeiterfassung &erfassung, QObject *parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;

    Zeiterfassung::Kontierung getKontierung(const QModelIndex &index) const;

    bool refresh(int userId, const QDate &from, const QDate &to);

    typedef QVector<Zeiterfassung::Kontierung>::iterator iterator;
    typedef QVector<Zeiterfassung::Kontierung>::const_iterator const_iterator;
    typedef QVector<Zeiterfassung::Kontierung>::reverse_iterator reverse_iterator;
    typedef QVector<Zeiterfassung::Kontierung>::const_reverse_iterator const_reverse_iterator;

#if !defined(QT_STRICT_ITERATORS) || defined(Q_QDOC)
    //inline iterator begin() { return m_kontierungen.begin(); }
    inline const_iterator begin() const Q_DECL_NOTHROW { return m_kontierungen.begin(); }
    inline const_iterator cbegin() const Q_DECL_NOTHROW { return m_kontierungen.cbegin(); }
    inline const_iterator constBegin() const Q_DECL_NOTHROW { return m_kontierungen.constBegin(); }
    //inline iterator end() { return m_kontierungen.end(); }
    inline const_iterator end() const Q_DECL_NOTHROW { return m_kontierungen.end(); }
    inline const_iterator cend() const Q_DECL_NOTHROW { return m_kontierungen.cend(); }
    inline const_iterator constEnd() const Q_DECL_NOTHROW { return m_kontierungen.constEnd(); }
#else
    //inline iterator begin(iterator = iterator()) { return m_kontierungen.begin(); }
    inline const_iterator begin(const_iterator = const_iterator()) const Q_DECL_NOTHROW { return m_kontierungen.begin(); }
    inline const_iterator cbegin(const_iterator = const_iterator()) const Q_DECL_NOTHROW { return d->constBegin(); }
    inline const_iterator constBegin(const_iterator = const_iterator()) const Q_DECL_NOTHROW { return m_kontierungen.constBegin(); }
    //inline iterator end(iterator = iterator()) { return m_kontierungen.end(); }
    inline const_iterator end(const_iterator = const_iterator()) const Q_DECL_NOTHROW { return m_kontierungen.end(); }
    inline const_iterator cend(const_iterator = const_iterator()) const Q_DECL_NOTHROW { return m_kontierungen.cend(); }
    inline const_iterator constEnd(const_iterator = const_iterator()) const Q_DECL_NOTHROW { return m_kontierungen.constEnd(); }
#endif
    //reverse_iterator rbegin() { return m_kontierungen.rbegin(); }
    //reverse_iterator rend() { return m_kontierungen.rbegin(); }
    const_reverse_iterator rbegin() const Q_DECL_NOTHROW { return m_kontierungen.rbegin(); }
    const_reverse_iterator rend() const Q_DECL_NOTHROW { return m_kontierungen.rend(); }
    const_reverse_iterator crbegin() const Q_DECL_NOTHROW { return m_kontierungen.crbegin(); }
    const_reverse_iterator crend() const Q_DECL_NOTHROW { return m_kontierungen.crend(); }

Q_SIGNALS:
    void refreshFinished(bool success, const QString &message);

private Q_SLOTS:
    void getKontierungenFinished(bool success, const QString &message, const QVector<Zeiterfassung::Kontierung> &kontierungen);

private:
    Zeiterfassung &m_erfassung;
    QVector<Zeiterfassung::Kontierung> m_kontierungen;
};

#endif // KONTIERUNGENMODEL_H
