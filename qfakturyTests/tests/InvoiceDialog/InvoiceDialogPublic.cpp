#include "InvoiceDialogPublic.h"
#include "CommodityDialogPublic.h"
#include "CommodityListDialogPublic.h"
#include "CounterpartyDialogPublic.h"

InvoiceDialogPublic::InvoiceDialogPublic(QWidget *parent, Database *db, InvoiceTypeData::Type invoiceType, const QModelIndex &idEdit)
    : InvoiceDialog(parent, db, invoiceType, idEdit)
{
    setObjectName("InvoiceDialogPublic");
}


CommodityDialogPublic* InvoiceDialogPublic::commodityDialog() const
{
    return static_cast<CommodityDialogPublic*>(pImpl_->commodityDialog);
}


CommodityListDialogPublic* InvoiceDialogPublic::commodityListDialog() const
{
    return static_cast<CommodityListDialogPublic*>(pImpl_->commodityListDialog);
}


CounterpartyDialogPublic* InvoiceDialogPublic::counterpartyDialog() const
{
    return static_cast<CounterpartyDialogPublic*>(pImpl_->counterpartyDialog);
}

Ui::InvoiceDialog* InvoiceDialogPublic::ui()
{
    return pImpl_->ui;
}
