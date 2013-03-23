#include "BillDialog.h"
#include "ui_InvoiceDialog.h"

// constructor
BillDialog::BillDialog(QWidget *parent, Database *db):
    QDialog(parent), ui_(new Ui::InvoiceDialog), db_(db)
{
    ui_->setupUi(this);
    init_();
}


BillDialog::~BillDialog()
{
    delete ui_;
}

void BillDialog::init_()
{
    setWindowTitle(InvoiceTypeData::name(InvoiceTypeData::BILL));
    ui_->comboBoxInvoiceType->setCurrentIndex(InvoiceTypeData::BILL - 1);
}
