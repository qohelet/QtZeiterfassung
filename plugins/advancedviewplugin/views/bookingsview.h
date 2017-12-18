#ifndef BOOKINGSVIEW_H
#define BOOKINGSVIEW_H

#include <QTreeView>

class BookingsView : public QTreeView
{
    Q_OBJECT

public:
    explicit BookingsView(QWidget *parent = Q_NULLPTR);
};

#endif // BOOKINGSVIEW_H
