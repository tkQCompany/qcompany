#include <QTest>
#include <QComboBox>
#include <QDebug>

#include "GuiUserAddNewCommodity.h"
#include "CommodityDialogPublic.h"
#include "CommodityDialog.cpp"
#include "InvoiceDialogPublic.h"

GuiUserAddNewCommodity::GuiUserAddNewCommodity(InvoiceDialogPublic *idp, const CommodityData &commodity, QObject *parent) :
    GuiUser(parent), idp_(idp), commodity_(commodity)
{
}


void GuiUserAddNewCommodity::process()
{
    CommodityDialogPublic *cd = 0;
    do
    {
        QTest::qWait(200);
        cd = idp_->commodityDialog();
    } while(cd == 0);

    postText_(cd->ui()->lineEditName, commodity_.field(CommodityFields::NAME).toString());
    postText_(cd->ui()->lineEditAbbreviation, commodity_.field(CommodityFields::ABBREV).toString());
    postText_(cd->ui()->lineEditPKWIU, commodity_.field(CommodityFields::PKWIU).toString());

    postComboBoxIndex_(cd->ui()->comboBoxType, commodity_.field(CommodityFields::TYPE_ID).toInt());
    postComboBoxIndex_(cd->ui()->comboBoxMeasureUnit, commodity_.field(CommodityFields::UNIT_ID).toInt());
    postComboBoxIndex_(cd->ui()->comboBoxVat, cd->ui()->comboBoxVat->findText(commodity_.field(CommodityFields::VAT).toString()));

    postText_(cd->ui()->lineEditNet1, commodity_.field(CommodityFields::NET1).toString());
    postText_(cd->ui()->lineEditNet2, commodity_.field(CommodityFields::NET2).toString());
    postText_(cd->ui()->lineEditNet3, commodity_.field(CommodityFields::NET3).toString());
    postText_(cd->ui()->lineEditNet4, commodity_.field(CommodityFields::NET4).toString());

    postDoubleVal_(cd->ui()->doubleSpinBoxQuantity, commodity_.field(CommodityFields::QUANTITY).toDouble());

    postMouseClick(cd->ui()->pushButtonOK);
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
