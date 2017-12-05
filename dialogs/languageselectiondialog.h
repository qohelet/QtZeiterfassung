#ifndef LANGUAGESELECTIONDIALOG_H
#define LANGUAGESELECTIONDIALOG_H

#include <QDialog>
#include <QLocale>

namespace Ui { class LanguageSelectionDialog; }

class LanguageSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LanguageSelectionDialog(QWidget *parent = 0);
    ~LanguageSelectionDialog();

    QLocale::Language language() const;
    void setLanguage(QLocale::Language language);

private:
    Ui::LanguageSelectionDialog *ui;
};

#endif // LANGUAGESELECTIONDIALOG_H
