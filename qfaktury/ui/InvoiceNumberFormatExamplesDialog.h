#ifndef INVOICENUMBERFORMATEXAMPLESDIALOG_H
#define INVOICENUMBERFORMATEXAMPLESDIALOG_H

#include <QDialog>
#include "Database.h"

namespace Ui {
class InvoiceNumberFormatExamplesDialog;
}

class InvoiceNumberFormatExamplesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit InvoiceNumberFormatExamplesDialog(QWidget *parent = 0, const QString &invoiceNumFormat = QString());
    ~InvoiceNumberFormatExamplesDialog();

private:
    void init_();
    
private:
    Ui::InvoiceNumberFormatExamplesDialog *ui;

    const QString invoiceNumFormat_;
    Database *db_;
};

#endif // INVOICENUMBERFORMATEXAMPLESDIALOG_H
