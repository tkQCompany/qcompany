#include <QTest>

#include "GuiUserAddCounterparty.h"
#include "CounterpartyDialogPublic.h"
#include "CounterpartyDialog.cpp"
#include "InvoiceDialogPublic.h"



GuiUserAddCounterparty::GuiUserAddCounterparty(InvoiceDialogPublic *idp, Counterparty_t *counterparty, QObject *parent) :
    GuiUser(parent), idp_(idp), counterparty_(*counterparty)
{
}


void GuiUserAddCounterparty::process()
{
    CounterpartyDialogPublic *cd = 0;
    do
    {
        cd = idp_->counterpartyDialog();
        QTest::qWait(200);
    } while(cd == 0);

    postText_(cd->ui()->lineEditName, counterparty_.name);

    postComboBoxIndex_(cd->ui()->comboBoxType, cd->ui()->comboBoxType->findText(counterparty_.type));
    postComboBoxIndex_(cd->ui()->comboBoxCountry, cd->ui()->comboBoxCountry->findText(counterparty_.country));

    postText_(cd->ui()->lineEditLocation, counterparty_.city);
    postText_(cd->ui()->lineEditPostalCode, counterparty_.postalCode);
    postText_(cd->ui()->lineEditAddress, counterparty_.street);
    postText_(cd->ui()->lineEditTaxID, counterparty_.taxID);
    postText_(cd->ui()->lineEditAccountName, counterparty_.accountNumber);
    postText_(cd->ui()->lineEditWWW, counterparty_.www);
    postText_(cd->ui()->lineEditPrimaryEmail, counterparty_.email);
    postText_(cd->ui()->lineEditPrimaryPhone, counterparty_.phone);

    postMouseClick(cd->ui()->pushButtonOK);
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
