#include "InvoiceNumberFormatExamplesDialog.h"
#include "ui_InvoiceNumberFormatExamplesDialog.h"

InvoiceNumberFormatExamplesDialog::InvoiceNumberFormatExamplesDialog(QWidget *parent, Database *db,
                                                                     const QString &invoiceNumFormat,
                                                                     const QString &counterpartyName) :
    QDialog(parent),
    ui(new Ui::InvoiceNumberFormatExamplesDialog),
    counterpartyName_(counterpartyName),
    invoiceNumFormat_(invoiceNumFormat),
    db_(db)
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
    for(int i = 0; i < size; ++i)
    {
        ptrDate.reset(new QTableWidgetItem(date.toString("yyyy-MM-dd")));
        ui->tableWidgetMain->setItem(i, COL_DATE, ptrDate.take());

        ptrExample.reset(new QTableWidgetItem(db_->modelInvoice()->generateInvoiceNumber(invoiceNumFormat_,
                                                                                         date,
                                                                                         InvoiceTypeData::name(InvoiceTypeData::VAT),
                                                                                         counterpartyName_)));
        ui->tableWidgetMain->setItem(i, COL_EXAMPLE, ptrExample.take());

        date = date.addDays(1);
    }
    ui->tableWidgetMain->setHorizontalHeaderLabels(QStringList() << trUtf8("Data") << trUtf8("Numeracja dla daty"));
    ui->tableWidgetMain->resizeColumnsToContents();
}
