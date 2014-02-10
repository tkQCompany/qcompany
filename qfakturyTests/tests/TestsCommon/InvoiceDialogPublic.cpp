#include "InvoiceDialogPublic.h"
#include "CommodityDialogPublic.h"
#include "../TestsCommon/CommodityListDialogPublic.h"
#include "CounterpartyDialogPublic.h"

InvoiceDialogPublic::InvoiceDialogPublic(QWidget *parent, Database *db, InvoiceTypeData::Type invoiceType, const QModelIndex &idEdit)
    : InvoiceDialog(parent, db, invoiceType, idEdit), DialogWithCommodityDialog(), DialogWithCommodityListDialog(),
      DialogWithCounterpartyDialog()
{
    QObject::setObjectName("InvoiceDialogPublic");
}


CommodityDialogPublic* InvoiceDialogPublic::commodityDialogPublic() const
{
    return static_cast<CommodityDialogPublic*>(pImpl_->commodityDialogPtr.data());
}


CommodityListDialogPublic* InvoiceDialogPublic::commodityListDialogPublic() const
{
    return static_cast<CommodityListDialogPublic*>(pImpl_->commodityListDialogPtr.data());
}


CounterpartyDialogPublic* InvoiceDialogPublic::counterpartyDialogPublic() const
{
    return static_cast<CounterpartyDialogPublic*>(pImpl_->counterpartyDialogPtr.data());
}

InvoiceDialogPublic* InvoiceDialogPublic::invoiceDialogPublic() const
{
    return 0;
}

Ui::InvoiceDialog* InvoiceDialogPublic::ui() const
{
    return pImpl_->ui;
}

InvoiceDialogImpl *InvoiceDialogPublic::implementation() const
{
    return pImpl_;
}
