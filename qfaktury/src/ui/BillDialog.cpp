#include "BillDialog.h"
#include "InvoiceDialogImpl.h"
#include "ui_InvoiceDialog.h"


class BillDialog::BillDialogImpl: public InvoiceDialogImpl
{
public:
    BillDialogImpl(QWidget *parent, Database *database) :
        InvoiceDialogImpl(parent, database) {}
};


BillDialog::BillDialog(QWidget *parent, Database *db):
    QDialog(parent), pImpl_(new BillDialogImpl(parent, db))
{
    pImpl_->ui->setupUi(this);
    pImpl_->init(InvoiceTypeData::BILL, QModelIndex());
}


BillDialog::~BillDialog()
{
    delete pImpl_;
}
