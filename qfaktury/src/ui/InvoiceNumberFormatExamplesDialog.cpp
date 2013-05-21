#include <QDate>

#include "InvoiceNumberFormatExamplesDialog.h"
#include "ui_InvoiceNumberFormatExamplesDialog.h"
#include "Database.h"
#include "ModelInvoice.h"
#include "InvoiceTypeData.h"
#include "SettingsGlobal.h"

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
    const QDate firstIssuanceDate(QDate::currentDate().year(), 1, 1);
    QScopedPointer<QTableWidgetItem> ptrDate, ptrExample;
    enum COLS {COL_DATE, COL_EXAMPLE};

    SettingsGlobal s;

    ui->tableWidgetMain->setRowCount(size);
    const QString invNumFormat(invoiceNumFormat_.isEmpty()
                               ?s.value(s.DEFAULT_INV_NUM_FORMAT).toString()
                              :invoiceNumFormat_);
    const QStringList simulatedNums(db_->modelInvoice()->
                                    simulateConsecutiveInvoiceNumbers(
                                        *InvoiceNumberFormat_t::Parse(invNumFormat).release(),
                                        firstIssuanceDate,
                                        InvoiceTypeData::VAT,
                                        size));
    QDate issuanceDate(firstIssuanceDate);
    for(int i = 0; i < size; ++i)
    {
        ptrDate.reset(new QTableWidgetItem(issuanceDate.toString("yyyy-MM-dd")));
        ui->tableWidgetMain->setItem(i, COL_DATE, ptrDate.take());

        ptrExample.reset(new QTableWidgetItem(simulatedNums.at(i)));
        ui->tableWidgetMain->setItem(i, COL_EXAMPLE, ptrExample.take());

        issuanceDate = issuanceDate.addDays(1);
    }
    ui->tableWidgetMain->setHorizontalHeaderLabels(QStringList() << trUtf8("Data") << trUtf8("Numeracja dla daty"));
    ui->tableWidgetMain->resizeColumnsToContents();
}
