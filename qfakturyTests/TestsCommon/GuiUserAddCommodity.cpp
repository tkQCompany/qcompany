#include <QTest>
#include <QDebug>

#include "GuiUserAddCommodity.h"
#include "ui_CommodityListDialog.h"
#include "../TestsCommon//CommodityListDialogPublic.h"
#include "../TestsCommon/DialogWithCommodityListDialog.h"
#include "CommodityListDialog.h"
#include "SettingsGlobal.h"


GuiUserAddCommodity::GuiUserAddCommodity(DialogWithCommodityListDialog *d, const CommodityData &commodity, const int whichNetVal, const DecVal &discount, QObject *parent) :
    GuiUser(parent), dialog_(d), commodity_(commodity), netValIndex_(whichNetVal), discount_(discount)
{
}


void GuiUserAddCommodity::process()
{
    bool ok = true;

    QPointer<CommodityListDialogPublic> cldp;
    do
    {
        QTest::qWait(200);
        cldp = dialog_->commodityListDialogPublic();
    } while(cldp == 0);

    connect(this, SIGNAL(setSpinBoxValue(int)), cldp->ui()->spinBoxDiscount, SLOT(setValue(int)), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(setDoubleValue(double)), cldp->ui()->doubleSpinBoxAmount, SLOT(setValue(double)), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(setCurrentIndex(const QModelIndex&)), cldp->ui()->listViewCommodities, SLOT(setCurrentIndex(const QModelIndex&)), Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(setComboBoxIndex(int)), cldp->ui()->comboBoxChosenNetPrice, SLOT(setCurrentIndex(int)), Qt::BlockingQueuedConnection);

    cldp->show();

    const int rowCount = cldp->ui()->listViewCommodities->model()->rowCount();
    if(rowCount > 0)
    {
        const QModelIndexList indList(cldp->ui()->listViewCommodities->model()->match(
                                          cldp->ui()->listViewCommodities->model()->index(0, CommodityFields::NAME),
                                          Qt::DisplayRole,
                                          commodity_.name()));
        if(!indList.isEmpty())
        {
            postListViewIndex_(indList.at(0));
            postComboBoxIndex_(netValIndex_);
            postDoubleVal_(commodity_.quantity().toDouble());
            postSpinBoxVal_(discount_);
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
        QTest::qWait(200);
        postMouseClick(cldp->ui()->pushButtonOK);
    }
    else
    {
        postMouseClick(cldp->ui()->pushButtonCancel);
    }

    do
    {
        QTest::qWait(200);
    } while(cldp);

    emit finished();
}


void GuiUserAddCommodity::postComboBoxIndex_(const int index)
{
    emit setComboBoxIndex(index);
}


void GuiUserAddCommodity::postListViewIndex_(const QModelIndex &index)
{
    emit setCurrentIndex(index);
}

void GuiUserAddCommodity::postDoubleVal_(const double val)
{
    emit setDoubleValue(val);
}


void GuiUserAddCommodity::postSpinBoxVal_(const DecVal &val)
{
    emit setSpinBoxValue((int)val.toDouble());
}
