#include <QPrinter>

#include "InvoiceDialog.h"
#include "ui_InvoiceDialog.h"
#include "Database.h"


InvoiceDialog::InvoiceDialog(QWidget *parent, Database *db, InvoiceTypeData::Type invoiceType, const QModelIndex &idEdit) :
    QDialog(parent), pImpl_(new InvoiceDialogImpl(this, db))
{
    pImpl_->ui->setupUi(this);
    pImpl_->init(invoiceType, idEdit);
}


/**
 * @brief
 *
 */
InvoiceDialog::~InvoiceDialog()
{
    delete pImpl_;
}
