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

    postText_(cdp->ui()->lineEditName, commodity_.name());
    postText_(cdp->ui()->lineEditAbbreviation, commodity_.abbrev());
    postText_(cdp->ui()->lineEditPKWIU, commodity_.pkwiu());

    postComboBoxIndex_(cdp->ui()->comboBoxType, commodity_.typeId());
    postComboBoxIndex_(cdp->ui()->comboBoxMeasureUnit, commodity_.unitId());
    postComboBoxIndex_(cdp->ui()->comboBoxVat, cdp->ui()->comboBoxVat->findText(DecVal::removeTrailingZeros(commodity_.vat().toString())));

    const int precision = 2;
    postText_(cdp->ui()->lineEditNet1, commodity_.net1().toString(precision));
    postText_(cdp->ui()->lineEditNet2, commodity_.net2().toString(precision));
    postText_(cdp->ui()->lineEditNet3, commodity_.net3().toString(precision));
    postText_(cdp->ui()->lineEditNet4, commodity_.net4().toString(precision));

    SettingsGlobal s;
    postDoubleVal_(cdp->ui()->doubleSpinBoxQuantity, s.stringToDouble(commodity_.quantity().toString()));

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
