#include <QTest>
#include <QDebug>

#include "GuiUserAddCommodity.h"
#include "../TestsCommon//CommodityListDialogPublic.h"
#include "../TestsCommon/DialogWithCommodityListDialog.h"
#include "CommodityListDialog.cpp"


GuiUserAddCommodity::GuiUserAddCommodity(DialogWithCommodityListDialog *d, const CommodityData &commodity, QObject *parent) :
    GuiUser(parent), dialog_(d), commodity_(commodity)
{
}


void GuiUserAddCommodity::process()
{
    bool ok = true;

    CommodityListDialogPublic *cldp = 0;
    do
    {
        QTest::qWait(200);
        cldp = dialog_->commodityListDialogPublic();
    } while(cldp == 0);

    const int rowCount = cldp->ui()->listViewCommodities->model()->rowCount();
    if(rowCount > 0)
    {
        const QModelIndexList indList(cldp->ui()->listViewCommodities->model()->match(
                                          cldp->ui()->listViewCommodities->model()->index(0, CommodityFields::NAME),
                                          Qt::DisplayRole,
                                          commodity_.field(CommodityFields::NAME).toString()));
        if(!indList.isEmpty())
        {
            postListViewIndex_(cldp->ui()->listViewCommodities, indList.at(0));
            postDoubleVal_(cldp->ui()->doubleSpinBoxAmount, commodity_.field(CommodityFields::QUANTITY).toDouble());
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
        postMouseClick(cldp->ui()->pushButtonOK);
    }
    else
    {
        postMouseClick(cldp->ui()->pushButtonCancel);
    }

    emit finished();
}


void GuiUserAddCommodity::postListViewIndex_(QListView *obj, const QModelIndex &index)
{
    connect(this, SIGNAL(setCurrentIndex(const QModelIndex&)), obj,
            SLOT(setCurrentIndex(const QModelIndex&)), Qt::BlockingQueuedConnection);
    emit setCurrentIndex(index);
    disconnect(this, SIGNAL(setCurrentIndex(const QModelIndex&)), obj,
            SLOT(setCurrentIndex(const QModelIndex&)));
}

void GuiUserAddCommodity::postDoubleVal_(QDoubleSpinBox *obj, const double val)
{
    connect(this, SIGNAL(setDoubleValue(double)), obj, SLOT(setValue(double)), Qt::BlockingQueuedConnection);
    emit setDoubleValue(val);
    disconnect(this, SIGNAL(setDoubleValue(double)), obj, SLOT(setValue(double)));
}
