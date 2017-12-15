#ifndef BOOKINGSMODEL_H
#define BOOKINGSMODEL_H

#include <QAbstractListModel>
#include <QVector>

#include "zeiterfassungapi.h"

class StripsWidget;

class BookingsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(StripsWidget* stripsWidget READ stripsWidget WRITE setStripsWidget NOTIFY stripsWidgetChanged)
    Q_PROPERTY(bool enabled READ enabled NOTIFY enabledChanged)

public:
    explicit BookingsModel(QObject *parent = Q_NULLPTR);

    StripsWidget *stripsWidget() const;
    void setStripsWidget(StripsWidget *stripsWidget);

    bool enabled() const;

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const Q_DECL_OVERRIDE;

Q_SIGNALS:
    void stripsWidgetChanged(StripsWidget *stripsWidget);
    void enabledChanged(bool enabled);

private Q_SLOTS:
    void bookingsChanged();
    void refreshingChanged(bool refreshing);

private:
    StripsWidget *m_stripsWidget;
    bool m_enabled;
};

#endif // BOOKINGSMODEL_H
