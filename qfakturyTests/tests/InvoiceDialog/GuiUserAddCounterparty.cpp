#include <QTest>

#include "GuiUserAddCounterparty.h"
#include "CounterpartyDialogPublic.h"
#include "CounterpartyDialog.cpp"
#include "InvoiceDialogPublic.h"



GuiUserAddCounterparty::GuiUserAddCounterparty(InvoiceDialogPublic *idp, Counterparty_t *counterparty, QObject *parent) :
    GuiUser(parent), idp_(idp), counterparty_(counterparty)
{
}


void GuiUserAddCounterparty::process()
{
    CounterpartyDialogPublic *cd = 0;
    do
    {
        cd = idp_->counterpartyDialog();
        QTest::qSleep(200);
    } while(cd == 0);

    QTest::keyClicks(cd->ui()->lineEditName, counterparty_->name);
    cd->ui()->comboBoxType->setCurrentIndex(cd->ui()->comboBoxType->findText(counterparty_->type));
    cd->ui()->comboBoxCountry->setCurrentIndex(cd->ui()->comboBoxCountry->findText(counterparty_->country));
    QTest::keyClicks(cd->ui()->lineEditLocation, counterparty_->city);
    QTest::keyClicks(cd->ui()->lineEditPostalCode, counterparty_->postalCode);
    QTest::keyClicks(cd->ui()->lineEditAddress, counterparty_->street);
    QTest::keyClicks(cd->ui()->lineEditTaxID, counterparty_->taxID);
    QTest::keyClicks(cd->ui()->lineEditAccountName, counterparty_->accountNumber);
    QTest::keyClicks(cd->ui()->lineEditWWW, counterparty_->www);
    QTest::keyClicks(cd->ui()->lineEditPrimaryEmail, counterparty_->email);
    QTest::keyClicks(cd->ui()->lineEditPrimaryPhone, counterparty_->phone);

    QMetaObject::invokeMethod(cd, "okClick");
    emit finished();
}
