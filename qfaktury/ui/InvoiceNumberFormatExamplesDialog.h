#ifndef INVOICENUMBERFORMATEXAMPLESDIALOG_H
#define INVOICENUMBERFORMATEXAMPLESDIALOG_H

#include <QDialog>

namespace Ui {
class InvoiceNumberFormatExamplesDialog;
}

class InvoiceNumberFormatExamplesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit InvoiceNumberFormatExamplesDialog(QWidget *parent = 0, const QString &format = QString());
    ~InvoiceNumberFormatExamplesDialog();
    
private:
    Ui::InvoiceNumberFormatExamplesDialog *ui;
};

#endif // INVOICENUMBERFORMATEXAMPLESDIALOG_H
