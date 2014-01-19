#ifndef MAINWINDOWPUBLIC_H
#define MAINWINDOWPUBLIC_H

#include "MainWindow.h"
#include "../TestsCommon/CommodityDialogPublic.h"
#include "../TestsCommon/DialogWithCommodityDialog.h"
#include "../TestsCommon/DialogWithCounterpartyDialog.h"

class MainWindowPublic: public MainWindow, public DialogWithCommodityDialog,
        public DialogWithCounterpartyDialog
{
    Q_OBJECT
public:
    MainWindowPublic(QWidget *parent = 0);

    CommodityDialogPublic* commodityDialogPublic() const;
    CommodityListDialogPublic* commodityListDialogPublic() const { return 0; }
    CounterpartyDialogPublic* counterpartyDialogPublic() const;
    Ui::MainWindow* ui();
    Database* database();
};

#endif
