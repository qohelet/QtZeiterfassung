#include "zeiterfassungdialog.h"

#include <qnamespace.h>

ZeiterfassungDialog::ZeiterfassungDialog(QWidget *parent) :
    QDialog(parent)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
#else
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
#endif
}
