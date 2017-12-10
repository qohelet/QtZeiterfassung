#include "bookingsmodel.h"

#include "replies/getbookingsreply.h"

BookingsModel::BookingsModel(ZeiterfassungApi &erfassung, QObject *parent) :
    QAbstractListModel(parent),
    m_erfassung(erfassung),
    m_reply(Q_NULLPTR)
{

}

int BookingsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_bookings.count();
}

int BookingsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 5;
}

QVariant BookingsModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.row() < m_bookings.count());
    const auto &booking = m_bookings.at(index.row());

    switch(role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        switch(index.column())
        {
        case 0: return booking.id;
        case 1: return booking.time;
        case 2: return booking.timespan;
        case 3: return booking.type;
        case 4: return booking.text;
        }
    }

    return QVariant();
}

QVariant BookingsModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            case 3: return tr("Type");
            case 4: return tr("Text");
            }
        }
    }

    return QVariant();
}

ZeiterfassungApi::Booking BookingsModel::getBooking(const QModelIndex &index) const
{
    if(!index.isValid())
        return ZeiterfassungApi::Booking();

    Q_ASSERT(index.row() <= m_bookings.count());
    return m_bookings.at(index.row());
}

void BookingsModel::refresh(int userId, const QDate &from, const QDate &to)
{
    beginResetModel();
    m_bookings.clear();
    endResetModel();

    m_reply = m_erfassung.doGetBookings(userId, from, to);
    connect(m_reply, &ZeiterfassungReply::finished, this, &BookingsModel::finished);
}

const QVector<ZeiterfassungApi::Booking> BookingsModel::bookings() const
{
    return m_bookings;
}

void BookingsModel::finished()
{
    if(m_reply->success())
    {
        beginResetModel();
        m_bookings = m_reply->bookings();
        endResetModel();
    }

    Q_EMIT refreshFinished(m_reply->success(), m_reply->message());

    m_reply->deleteLater();
    m_reply = Q_NULLPTR;
}
