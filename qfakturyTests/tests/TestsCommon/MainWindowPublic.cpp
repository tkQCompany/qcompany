#include "MainWindowPublic.h"
#include "../TestsCommon/CounterpartyDialogPublic.h"

MainWindowPublic::MainWindowPublic(QWidget *parent)
    : MainWindow(parent), DialogWithCommodityDialog(),
      DialogWithCounterpartyDialog()
{
}



CommodityDialogPublic* MainWindowPublic::commodityDialogPublic() const
{
    return static_cast<CommodityDialogPublic*>(commodityDialogPtr.data());
}

CounterpartyDialogPublic* MainWindowPublic::counterpartyDialogPublic() const
{
    return dynamic_cast<CounterpartyDialogPublic*>(counterpartyDialogPtr.data());
}


Ui::MainWindow* MainWindowPublic::ui()
{
    return MainWindow::ui;
}

Database* MainWindowPublic::database()
{
    return MainWindow::database();
}
