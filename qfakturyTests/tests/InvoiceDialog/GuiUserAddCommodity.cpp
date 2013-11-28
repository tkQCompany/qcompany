#include <QTest>
#include <QDebug>

#include "GuiUserAddCommodity.h"
#include "../CommodityListDialog/CommodityListDialogPublic.cpp"
#include "CommodityListDialog.cpp"
#include "InvoiceDialogPublic.h"


GuiUserAddCommodity::GuiUserAddCommodity(InvoiceDialogPublic *idp, const CommodityData *commodity, QObject *parent) :
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

    bool ok = true;

    const int rowCount = cld->ui()->listViewCommodities->model()->rowCount();
    if(rowCount > 0)
    {
        const QModelIndexList indList(cld->ui()->listViewCommodities->model()->match(
                                          cld->ui()->listViewCommodities->model()->index(0, CommodityFields::NAME),
                                          Qt::DisplayRole,
                                          commodity_->field(CommodityFields::NAME).toString()));
        if(!indList.isEmpty())
        {
            cld->ui()->listViewCommodities->setCurrentIndex(indList.at(0));
            cld->ui()->doubleSpinBoxAmount->setValue(commodity_->field(CommodityFields::QUANTITY).toDouble());
        }
        else
        {
            ok = false;
            qDebug("GuiUserAddCommodity::process(): empty result of matching a commodity name. Number of items in the listWidget: %d", rowCount);
        }
    }
    else
    {
        ok = false;
        qDebug("GuiUserAddCommodity::process(): empty cld->ui()->listViewCommodities");
    }

    if(ok)
    {
        QMetaObject::invokeMethod(cld, "doAccept");
    }
    else
    {
        QMetaObject::invokeMethod(cld, "close");
    }

    emit finished();
}
