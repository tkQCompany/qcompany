#ifndef INVOICENUMBERFORMATEXAMPLESDIALOG_H
#define INVOICENUMBERFORMATEXAMPLESDIALOG_H

#include <QDialog>

namespace Ui {
class InvoiceNumberFormatExamplesDialog;
}


class Database;


class InvoiceNumberFormatExamplesDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit InvoiceNumberFormatExamplesDialog(QWidget *parent, Database *db, const QString &invoiceNumFormat = QString(), const QString &counterpartyName = QString());
    ~InvoiceNumberFormatExamplesDialog();

private:
    void init_();
    
private:
    Ui::InvoiceNumberFormatExamplesDialog *ui;
    const QString counterpartyName_;
    const QString invoiceNumFormat_;
    Database *db_;
};

#endif // INVOICENUMBERFORMATEXAMPLESDIALOG_H
