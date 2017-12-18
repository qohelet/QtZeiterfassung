#include "bookingsmodel.h"

#include "stripswidget.h"

BookingsModel::BookingsModel(StripsWidget &stripsWidget, QObject *parent) :
    QAbstractListModel(parent),
    m_stripsWidget(stripsWidget)
{
    connect(&stripsWidget, &StripsWidget::bookingsChanged, this, &BookingsModel::bookingsChanged);
    connect(&stripsWidget, &StripsWidget::refreshingBookingsChanged, this, &BookingsModel::enabledChanged);
}

StripsWidget &BookingsModel::stripsWidget() const
{
    return m_stripsWidget;
}

bool BookingsModel::enabled() const
{
    return !m_stripsWidget.refreshingBookings();
}

int BookingsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_stripsWidget.bookings().count();
}

int BookingsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 5;
}

QVariant BookingsModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.row() < m_stripsWidget.bookings().count());
    const auto &booking = m_stripsWidget.bookings().at(index.row());

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
    default:
        qt_noop();
    }

    return QVariant();
}

void BookingsModel::bookingsChanged()
{
    beginResetModel();
    endResetModel();
}
