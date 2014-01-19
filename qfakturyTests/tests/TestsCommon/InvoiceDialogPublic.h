#ifndef INVOICEDIALOGPUBLIC_H
#define INVOICEDIALOGPUBLIC_H

#include "InvoiceDialog.h"
#include "ui_InvoiceDialog.h"
#include "../TestsCommon/DialogWithCommodityDialog.h"
#include "../TestsCommon/DialogWithCommodityListDialog.h"
#include "../TestsCommon/DialogWithCounterpartyDialog.h"


class CommodityDialogPublic;
class CommodityListDialogPublic;
class CounterpartyDialogPublic;

class InvoiceDialogPublic: public InvoiceDialog, public DialogWithCommodityDialog,
        public DialogWithCommodityListDialog, public DialogWithCounterpartyDialog
{
    Q_OBJECT
public:
    InvoiceDialogPublic(QWidget *parent, Database *db, InvoiceTypeData::Type invoiceType, const QModelIndex &idEdit = QModelIndex());
    CommodityDialogPublic* commodityDialogPublic() const;
    CommodityListDialogPublic* commodityListDialogPublic() const;
    CounterpartyDialogPublic* counterpartyDialogPublic() const;
    Ui::InvoiceDialog *ui();
};

#endif // INVOICEDIALOGPUBLIC_H
