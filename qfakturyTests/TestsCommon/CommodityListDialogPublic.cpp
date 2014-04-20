#include "CommodityListDialogPublic.h"

CommodityListDialogPublic::CommodityListDialogPublic(QWidget *parent, Database *db) :
    CommodityListDialog(parent, db)
{
    setObjectName("CommodityListDialogPublic");
}


Ui::CommodityListDialog* CommodityListDialogPublic::ui()
{
    return CommodityListDialog::ui;
}

QDataWidgetMapper* CommodityListDialogPublic::getDataWidgetMapper() const
{
    return widgetMapper;
}
