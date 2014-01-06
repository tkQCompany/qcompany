#include "InvoiceDialogPublic.h"
#include "CommodityDialogPublic.h"
#include "../CommodityListDialog/CommodityListDialogPublic.h"
#include "CounterpartyDialogPublic.h"

InvoiceDialogPublic::InvoiceDialogPublic(QWidget *parent, Database *db, InvoiceTypeData::Type invoiceType, const QModelIndex &idEdit)
    : InvoiceDialog(parent, db, invoiceType, idEdit)
{
    setObjectName("InvoiceDialogPublic");
}


CommodityDialogPublic* InvoiceDialogPublic::commodityDialog() const
{
    return static_cast<CommodityDialogPublic*>(pImpl_->commodityDialogPtr.data());
}


CommodityListDialogPublic* InvoiceDialogPublic::commodityListDialog() const
{
    return static_cast<CommodityListDialogPublic*>(pImpl_->commodityListDialogPtr.data());
}


CounterpartyDialogPublic* InvoiceDialogPublic::counterpartyDialog() const
{
    return static_cast<CounterpartyDialogPublic*>(pImpl_->counterpartyDialogPtr.data());
}

Ui::InvoiceDialog* InvoiceDialogPublic::ui()
{
    return pImpl_->ui;
}
