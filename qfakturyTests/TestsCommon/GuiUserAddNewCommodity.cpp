#include <QTest>
#include <QComboBox>
#include <QDebug>

#include "SettingsGlobal.h"
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
    QPointer<CommodityDialogPublic> cdp;
    do
    {
        QTest::qWait(200);
        cdp = dialog_->commodityDialogPublic();
    } while(cdp == 0);

    cdp->show();

    postText_(cdp->ui()->lineEditName, commodity_.field(CommodityFields::NAME).toString());
    postText_(cdp->ui()->lineEditAbbreviation, commodity_.field(CommodityFields::ABBREV).toString());
    postText_(cdp->ui()->lineEditPKWIU, commodity_.field(CommodityFields::PKWIU).toString());

    postComboBoxIndex_(cdp->ui()->comboBoxType, commodity_.field(CommodityFields::TYPE_ID).toInt());
    postComboBoxIndex_(cdp->ui()->comboBoxMeasureUnit, commodity_.field(CommodityFields::UNIT_ID).toInt());
    postComboBoxIndex_(cdp->ui()->comboBoxVat, cdp->ui()->comboBoxVat->findText(DecVal::removeTrailingZeros(commodity_.field(CommodityFields::VAT).value<DecVal>().toString())));

    const int precision = 2;
    postText_(cdp->ui()->lineEditNet1, commodity_.field(CommodityFields::NET1).value<Money_t>().toString(precision));
    postText_(cdp->ui()->lineEditNet2, commodity_.field(CommodityFields::NET2).value<Money_t>().toString(precision));
    postText_(cdp->ui()->lineEditNet3, commodity_.field(CommodityFields::NET3).value<Money_t>().toString(precision));
    postText_(cdp->ui()->lineEditNet4, commodity_.field(CommodityFields::NET4).value<Money_t>().toString(precision));

    SettingsGlobal s;
    postDoubleVal_(cdp->ui()->doubleSpinBoxQuantity, s.stringToDouble(commodity_.field(CommodityFields::QUANTITY).value<DecVal>().toString()));

    postMouseClick(cdp->ui()->pushButtonOK);

    do
    {
        QTest::qWait(200);
    } while(cdp);

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
