#include <QTest>

#include "GuiUserAddNewCommodity.h"
#include "CommodityDialogPublic.h"
#include "CommodityDialog.cpp"
#include "InvoiceDialogPublic.h"

GuiUserAddNewCommodity::GuiUserAddNewCommodity(InvoiceDialogPublic *idp, const CommodityData *commodity, QObject *parent) :
    GuiUser(parent), idp_(idp), commodity_(commodity)
{
}


void GuiUserAddNewCommodity::process()
{
    CommodityDialogPublic *cd = 0;
    do
    {
        QTest::qSleep(200);
        cd = idp_->commodityDialog();
    } while(cd == 0);

    cd->ui()->lineEditName->setText(commodity_->field(CommodityFields::NAME).toString());
    cd->ui()->lineEditAbbreviation->setText(commodity_->field(CommodityFields::ABBREV).toString());
    cd->ui()->lineEditPKWIU->setText(commodity_->field(CommodityFields::PKWIU).toString());
    cd->ui()->comboBoxType->setCurrentIndex(commodity_->field(CommodityFields::TYPE_ID).toInt());
    cd->ui()->comboBoxMeasureUnit->setCurrentIndex(commodity_->field(CommodityFields::UNIT_ID).toInt());
    cd->ui()->comboBoxVat->setCurrentIndex(cd->ui()->comboBoxVat->findText(commodity_->field(CommodityFields::VAT).toString()));
    cd->ui()->lineEditNet1->setText(commodity_->field(CommodityFields::NET1).toString());
    cd->ui()->lineEditNet2->setText(commodity_->field(CommodityFields::NET2).toString());
    cd->ui()->lineEditNet3->setText(commodity_->field(CommodityFields::NET3).toString());
    cd->ui()->lineEditNet4->setText(commodity_->field(CommodityFields::NET4).toString());
    cd->ui()->doubleSpinBoxQuantity->setValue(commodity_->field(CommodityFields::QUANTITY).toDouble());

    QMetaObject::invokeMethod(cd, "okClick");
    emit finished();
}
