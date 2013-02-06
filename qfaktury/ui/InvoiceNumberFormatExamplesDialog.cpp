#include "InvoiceNumberFormatExamplesDialog.h"
#include "ui_InvoiceNumberFormatExamplesDialog.h"

InvoiceNumberFormatExamplesDialog::InvoiceNumberFormatExamplesDialog(QWidget *parent, const QString &invoiceNumFormat) :
    QDialog(parent),
    ui(new Ui::InvoiceNumberFormatExamplesDialog),
    invoiceNumFormat_(invoiceNumFormat)
{
    ui->setupUi(this);
    init_();
}

InvoiceNumberFormatExamplesDialog::~InvoiceNumberFormatExamplesDialog()
{
    delete ui;
}


void InvoiceNumberFormatExamplesDialog::init_()
{
    const int size = 370; //365 days in a year + several days
    QDate date(QDate::currentDate().year(), 1, 1);
    QScopedPointer<QTableWidgetItem> ptrDate, ptrExample;
    enum COLS {COL_DATE, COL_EXAMPLE};

    ui->tableWidgetMain->setRowCount(size);
    ui->tableWidgetMain->setColumnCount(2);
    for(int i = 0; i < size; ++i)
    {
        ptrDate.reset(new QTableWidgetItem(date.toString("yyyy-MM-dd")));
        ui->tableWidgetMain->setItem(i, COL_DATE, ptrDate.take());

        ptrExample.reset(new QTableWidgetItem(db_->modelInvoice()->generateInvoiceNumber(invoiceNumFormat_,
                                                                                         date,
                                                                                         InvoiceTypeData::InvoiceTypeToString(InvoiceTypeData::VAT))));
        ui->tableWidgetMain->setItem(i, COL_EXAMPLE, ptrExample.take());

        date = date.addDays(1);
    }
    ui->tableWidgetMain->resizeColumnsToContents();
}
