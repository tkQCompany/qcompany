#ifndef INVOICEDIALOGPUBLIC_H
#define INVOICEDIALOGPUBLIC_H

#include "InvoiceDialog.h"
#include "ui_InvoiceDialog.h"


class CommodityDialogPublic;
class CommodityListDialogPublic;
class CounterpartyDialogPublic;

class InvoiceDialogPublic: public InvoiceDialog
{
    Q_OBJECT
public:
    InvoiceDialogPublic(QWidget *parent, Database *db, InvoiceTypeData::Type invoiceType, const QModelIndex &idEdit = QModelIndex());
    CommodityDialogPublic* commodityDialog() const;
    CommodityListDialogPublic* commodityListDialog() const;
    CounterpartyDialogPublic* counterpartyDialog() const;
    Ui::InvoiceDialog *ui();
};

#endif // INVOICEDIALOGPUBLIC_H
