#ifndef ADVANVEDVIEWDIALOG_H
#define ADVANVEDVIEWDIALOG_H

#include <QDialog>

class StripsWidget;
namespace Ui { class AdvanvedViewDialog; }

class AdvanvedViewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdvanvedViewDialog(StripsWidget &stripsWidget);
    ~AdvanvedViewDialog();

private:
    Ui::AdvanvedViewDialog *ui;

    StripsWidget &m_stripsWidget;
};

#endif // ADVANVEDVIEWDIALOG_H
