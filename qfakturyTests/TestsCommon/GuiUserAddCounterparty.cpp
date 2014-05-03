#include <QTest>

#include "GuiUserAddCounterparty.h"
#include "CounterpartyDialogPublic.h"
#include "CounterpartyDialog.cpp"
#include "../TestsCommon/DialogWithCounterpartyDialog.h"


GuiUserAddCounterparty::GuiUserAddCounterparty(DialogWithCounterpartyDialog *d, const Counterparty_t &counterparty, QObject *parent) :
    GuiUser(parent), dialog_(d), counterparty_(counterparty)
{
}


void GuiUserAddCounterparty::process()
{
    QPointer<CounterpartyDialogPublic> cdp;
    do
    {
        QTest::qWait(200);
        cdp = dialog_->counterpartyDialogPublic();
    } while(cdp == 0);

    cdp->show();

    postText_(cdp->ui()->lineEditName, counterparty_.name);

    postComboBoxIndex_(cdp->ui()->comboBoxType, cdp->ui()->comboBoxType->findText(counterparty_.type));
    postComboBoxIndex_(cdp->ui()->comboBoxCountry, cdp->ui()->comboBoxCountry->findText(counterparty_.country));

    postText_(cdp->ui()->lineEditLocation, counterparty_.city);
    postText_(cdp->ui()->lineEditPostalCode, counterparty_.postalCode);
    postText_(cdp->ui()->lineEditAddress, counterparty_.street);
    postText_(cdp->ui()->lineEditTaxID, counterparty_.taxID);
    postText_(cdp->ui()->lineEditAccountName, counterparty_.accountNumber);
    postText_(cdp->ui()->lineEditWWW, counterparty_.www);
    postText_(cdp->ui()->lineEditPrimaryEmail, counterparty_.email);
    postText_(cdp->ui()->lineEditPrimaryPhone, counterparty_.phone);

    postMouseClick(cdp->ui()->pushButtonOK);

    do
    {
        QTest::qWait(200);
    } while(cdp);

    emit finished();
}

void GuiUserAddCounterparty::postText_(QObject *obj, const QString &str)
{
    QCoreApplication::postEvent(obj, new QKeyEvent(QEvent::KeyPress, 0, Qt::NoModifier, str, false, str.size()));
}

void GuiUserAddCounterparty::postComboBoxIndex_(QComboBox *obj, const int index)
{
    connect(this, SIGNAL(setComboBoxIndex(int)), obj, SLOT(setCurrentIndex(int)), Qt::BlockingQueuedConnection);
    emit setComboBoxIndex(index);
    disconnect(this, SIGNAL(setComboBoxIndex(int)), obj, SLOT(setCurrentIndex(int)));
}
