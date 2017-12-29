#pragma once

#include <QLocale>

#include "zeiterfassungguilib_global.h"
#include "zeiterfassungdialog.h"

namespace Ui { class LanguageSelectionDialog; }

class ZEITERFASSUNGGUILIBSHARED_EXPORT LanguageSelectionDialog : public ZeiterfassungDialog
{
    Q_OBJECT

public:
    explicit LanguageSelectionDialog(QWidget *parent = Q_NULLPTR);
    ~LanguageSelectionDialog();

    QLocale::Language language() const;
    void setLanguage(QLocale::Language language);

private:
    Ui::LanguageSelectionDialog *ui;
};
