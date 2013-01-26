#include "InvoiceNumberFormatExamplesDialog.h"
#include "ui_InvoiceNumberFormatExamplesDialog.h"

InvoiceNumberFormatExamplesDialog::InvoiceNumberFormatExamplesDialog(QWidget *parent, const QString &format) :
    QDialog(parent),
    ui(new Ui::InvoiceNumberFormatExamplesDialog)
{
    ui->setupUi(this);
}

InvoiceNumberFormatExamplesDialog::~InvoiceNumberFormatExamplesDialog()
{
    delete ui;
}
