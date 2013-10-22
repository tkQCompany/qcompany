#include <QTest>

#include "GuiUserAddNewCommodity.h"
#include "CommodityDialogPublic.h"
#include "CommodityDialog.cpp"
#include "InvoiceDialogPublic.h"

GuiUserAddNewCommodity::GuiUserAddNewCommodity(InvoiceDialogPublic *idp, const CommodityVisualData *commodity, QObject *parent) :
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

    cd->ui()->lineEditName->setText(commodity_->name);
    cd->ui()->lineEditPKWIU->setText(commodity_->pkwiu);
    cd->ui()->comboBoxType->setCurrentIndex(cd->ui()->comboBoxType->findText(commodity_->type));
    cd->ui()->comboBoxMeasureUnit->setCurrentIndex(cd->ui()->comboBoxMeasureUnit->findText(commodity_->unit));
    cd->ui()->comboBoxVat->setCurrentIndex(cd->ui()->comboBoxVat->findText(commodity_->vat));
    cd->ui()->lineEditNet1->setText(commodity_->net);

    QMetaObject::invokeMethod(cd, "okClick");
    emit finished();
}
