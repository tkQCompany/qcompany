#include <QTest>
#include <QComboBox>
#include <QDebug>

#include "GuiUserAddNewCommodity.h"
#include "CommodityDialogPublic.h"
#include "CommodityDialog.cpp"
#include "../TestsCommon/DialogWithCommodityDialog.h"

GuiUserAddNewCommodity::GuiUserAddNewCommodity(DialogWithCommodityDialog *d, const CommodityData &commodity, QObject *parent) :
    GuiUser(parent), dialog_(d), commodity_(commodity)
{
}


void GuiUserAddNewCommodity::process()
{
    CommodityDialogPublic *cdp = 0;
    do
    {
        QTest::qWait(200);
        cdp = dialog_->commodityDialogPublic();
    } while(cdp == 0);

    postText_(cdp->ui()->lineEditName, commodity_.field(CommodityFields::NAME).toString());
    postText_(cdp->ui()->lineEditAbbreviation, commodity_.field(CommodityFields::ABBREV).toString());
    postText_(cdp->ui()->lineEditPKWIU, commodity_.field(CommodityFields::PKWIU).toString());

    postComboBoxIndex_(cdp->ui()->comboBoxType, commodity_.field(CommodityFields::TYPE_ID).toInt());
    postComboBoxIndex_(cdp->ui()->comboBoxMeasureUnit, commodity_.field(CommodityFields::UNIT_ID).toInt());
    postComboBoxIndex_(cdp->ui()->comboBoxVat, cdp->ui()->comboBoxVat->findText(commodity_.field(CommodityFields::VAT).toString()));

    postText_(cdp->ui()->lineEditNet1, commodity_.field(CommodityFields::NET1).toString());
    postText_(cdp->ui()->lineEditNet2, commodity_.field(CommodityFields::NET2).toString());
    postText_(cdp->ui()->lineEditNet3, commodity_.field(CommodityFields::NET3).toString());
    postText_(cdp->ui()->lineEditNet4, commodity_.field(CommodityFields::NET4).toString());

    postDoubleVal_(cdp->ui()->doubleSpinBoxQuantity, commodity_.field(CommodityFields::QUANTITY).toDouble());

    postMouseClick(cdp->ui()->pushButtonOK);
    emit finished();
}

void GuiUserAddNewCommodity::postText_(QObject *obj, const QString &str)
{
    QCoreApplication::postEvent(obj, new QKeyEvent(QEvent::KeyPress, 0, Qt::NoModifier, str, false, str.size()));
}

void GuiUserAddNewCommodity::postComboBoxIndex_(QComboBox *obj, const int index)
{
    connect(this, SIGNAL(setComboBoxIndex(int)), obj, SLOT(setCurrentIndex(int)), Qt::BlockingQueuedConnection);
    emit setComboBoxIndex(index);
    disconnect(this, SIGNAL(setComboBoxIndex(int)), obj, SLOT(setCurrentIndex(int)));
}

void GuiUserAddNewCommodity::postDoubleVal_(QDoubleSpinBox *obj, const double val)
{
    connect(this, SIGNAL(setDoubleValue(double)), obj, SLOT(setValue(double)), Qt::BlockingQueuedConnection);
    emit setDoubleValue(val);
    disconnect(this, SIGNAL(setDoubleValue(double)), obj, SLOT(setValue(double)));
}
