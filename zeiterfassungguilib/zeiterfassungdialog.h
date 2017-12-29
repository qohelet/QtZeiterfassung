#pragma once

#include <QDialog>

#include "zeiterfassungguilib_global.h"

class ZEITERFASSUNGGUILIBSHARED_EXPORT ZeiterfassungDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ZeiterfassungDialog(QWidget *parent = Q_NULLPTR);
};
