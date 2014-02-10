#ifndef MAINWINDOWPUBLIC_H
#define MAINWINDOWPUBLIC_H

#define private public

#include "MainWindow.h"
#include "../TestsCommon/CommodityDialogPublic.h"
#include "../TestsCommon/DialogWithCommodityDialog.h"
#include "../TestsCommon/DialogWithCounterpartyDialog.h"
#include "../TestsCommon/DialogWithInvoiceDialog.h"
#include "../TestsCommon/InvoiceDialogPublic.h"

class MainWindowPublic: public MainWindow, public DialogWithCommodityDialog,
        public DialogWithCounterpartyDialog, public DialogWithInvoiceDialog
{
    Q_OBJECT
public:
    MainWindowPublic(QWidget *parent = 0);

    CommodityDialogPublic *commodityDialogPublic() const;
    CommodityListDialogPublic* commodityListDialogPublic() const { return 0; }
    CounterpartyDialogPublic* counterpartyDialogPublic() const;
    InvoiceDialogPublic* invoiceDialogPublic() const;
    Ui::MainWindow* ui();
    Database* database();
};

#endif
