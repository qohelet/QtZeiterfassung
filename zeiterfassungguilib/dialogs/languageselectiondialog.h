#pragma once

#include <QDialog>
#include <QLocale>

#include "zeiterfassungguilib_global.h"

namespace Ui { class LanguageSelectionDialog; }

class ZEITERFASSUNGGUILIBSHARED_EXPORT LanguageSelectionDialog : public QDialog
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
