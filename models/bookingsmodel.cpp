#include "bookingsmodel.h"

#include <QDebug>

#include "stripswidget.h"

BookingsModel::BookingsModel(QObject *parent) :
    QAbstractListModel(parent),
    m_stripsWidget(Q_NULLPTR),
    m_enabled(false)
{
}

StripsWidget *BookingsModel::stripsWidget() const
{
    return m_stripsWidget;
}

void BookingsModel::setStripsWidget(StripsWidget *stripsWidget)
{
    if(m_stripsWidget != stripsWidget)
    {
        if(m_stripsWidget)
        {
            disconnect(m_stripsWidget, &StripsWidget::bookingsChanged, this, &BookingsModel::bookingsChanged);
            disconnect(m_stripsWidget, &StripsWidget::refreshingBookingsChanged, this, &BookingsModel::refreshingChanged);
        }

        beginResetModel();
        Q_EMIT stripsWidgetChanged(m_stripsWidget = stripsWidget);
        endResetModel();

        if(m_stripsWidget)
        {
            connect(m_stripsWidget, &StripsWidget::bookingsChanged, this, &BookingsModel::bookingsChanged);
            connect(m_stripsWidget, &StripsWidget::refreshingBookingsChanged, this, &BookingsModel::refreshingChanged);

            if(m_enabled == m_stripsWidget->refreshingBookings())
                Q_EMIT enabledChanged(m_enabled = !m_stripsWidget->refreshingBookings());
        }
        else if(m_enabled)
            Q_EMIT enabledChanged(m_enabled = false);
    }
}

bool BookingsModel::enabled() const
{
    return m_enabled;
}

int BookingsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_stripsWidget ? m_stripsWidget->bookings().count() : 0;
}

int BookingsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 5;
}

QVariant BookingsModel::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(m_stripsWidget != Q_NULLPTR);
    Q_ASSERT(index.row() < m_stripsWidget->bookings().count());
    const auto &booking = m_stripsWidget->bookings().at(index.row());

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

void BookingsModel::bookingsChanged()
{
    beginResetModel();
    endResetModel();
}

void BookingsModel::refreshingChanged(bool refreshing)
{
    if(m_enabled == refreshing)
        Q_EMIT enabledChanged(m_enabled = !refreshing);
}
