#include "CommodityDialogPublic.h"

CommodityDialogPublic::CommodityDialogPublic(QWidget *parent, Database *db, const QModelIndex &id_edit)
    : CommodityDialog(parent, db, id_edit)
{
    setObjectName("CommodityDialogPublic");
}


Ui::CommodityDialog* CommodityDialogPublic::ui()
{
    return CommodityDialog::ui;
}
