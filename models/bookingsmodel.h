#ifndef BOOKINGSMODEL_H
#define BOOKINGSMODEL_H

#include <QAbstractListModel>
#include <QVector>

#include "zeiterfassung.h"

class BookingsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit BookingsModel(Zeiterfassung &erfassung, QObject *parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;

    Zeiterfassung::Booking getBooking(const QModelIndex &index) const;

    bool refresh(int userId, const QDate &from, const QDate &to);

    const QVector<Zeiterfassung::Booking> bookings() const;

Q_SIGNALS:
    void refreshFinished(bool success, const QString &message);

private Q_SLOTS:
    void getBookingsFinished(bool success, const QString &message, const QVector<Zeiterfassung::Booking> &bookings);

private:
    Zeiterfassung &m_erfassung;
    QVector<Zeiterfassung::Booking> m_bookings;
};

#endif // BOOKINGSMODEL_H
