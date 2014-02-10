#ifndef DIALOGWITHPUBLICDIALOG_H
#define DIALOGWITHPUBLICDIALOG_H

class CommodityDialogPublic;
class CommodityListDialogPublic;
class CounterpartyDialogPublic;
class InvoiceDialogPublic;

class DialogWithPublicDialog
{
public:
    virtual CommodityDialogPublic* commodityDialogPublic() const = 0;
    virtual CommodityListDialogPublic* commodityListDialogPublic() const = 0;
    virtual CounterpartyDialogPublic* counterpartyDialogPublic() const = 0;
    virtual InvoiceDialogPublic* invoiceDialogPublic() const = 0;
};

#endif // DIALOGWITHPUBLICDIALOG_H
