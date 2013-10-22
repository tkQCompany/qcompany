#include <QTest>

#include "GuiUserAddCommodity.h"
#include "CommodityListDialogPublic.h"
#include "CommodityListDialog.cpp"
#include "InvoiceDialogPublic.h"


GuiUserAddCommodity::GuiUserAddCommodity(InvoiceDialogPublic *idp, const CommodityVisualData *commodity, QObject *parent) :
    GuiUser(parent), idp_(idp), commodity_(commodity)
{
}


void GuiUserAddCommodity::process()
{
    CommodityListDialogPublic *cld = 0;
    do
    {
        QTest::qSleep(200);
        cld = idp_->commodityListDialog();
    } while(cld == 0);

    cld->ui()->listViewCommodities->setCurrentIndex(cld->ui()->listViewCommodities->model()->match(
                                                        cld->ui()->listViewCommodities->model()->index(0, CommodityFields::NAME),
                                                        Qt::DisplayRole,
                                                        commodity_->name).at(0));
    cld->ui()->doubleSpinBoxAmount->setValue(commodity_->quantity.toInt());
    QMetaObject::invokeMethod(cld, "doAccept");
    emit finished();
}
