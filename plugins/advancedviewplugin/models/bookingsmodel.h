#ifndef BOOKINGSMODEL_H
#define BOOKINGSMODEL_H

#include <QAbstractListModel>
#include <QVector>

#include "zeiterfassungapi.h"

class StripsWidget;

class BookingsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled NOTIFY enabledChanged)

public:
    explicit BookingsModel(StripsWidget &stripsWidget, QObject *parent = Q_NULLPTR);

    StripsWidget &stripsWidget() const;

    bool enabled() const;

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;

Q_SIGNALS:
    void enabledChanged(bool enabled);

private Q_SLOTS:
    void bookingsChanged();
    void refreshingChanged(bool refreshing);

private:
    StripsWidget &m_stripsWidget;
};

#endif // BOOKINGSMODEL_H
