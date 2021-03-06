#include <QTest>
#include <QDebug>

#include "GuiUserEditInvoice.h"
#include "InvoiceDialogPublic.h"
#include "MainWindowPublic.h"

GuiUserEditInvoice::GuiUserEditInvoice(MainWindowPublic *mwp, QObject *parent) :
    GuiUser(parent), mwp_(mwp)
{
}


void GuiUserEditInvoice::process()
{
    InvoiceDialogPublic *idp = 0;
    do
    {
        QTest::qWait(200);
        idp = mwp_->invoiceDialogPublic();
    } while(idp == 0);

    invData_ = idp->implementation()->getInvoiceData();
    commodities_ = idp->implementation()->getCommoditiesVisualData();
    totalNetVal_ = Money_t(idp->ui()->labelSumNetVal->text());
    totalDiscountVal_ = Money_t(idp->ui()->labelDiscountVal->text());
    totalGrossVal_ = Money_t(idp->ui()->labelSumGrossVal->text());

    if(!idp->ui()->pushButtonMoreInfo->isEnabled())
    {
        QWARN("GuiUserEditInvoice::process(): !idp->ui()->pushButtonMoreInfo->isEnabled()");
    }

    postMouseClick(idp->ui()->pushButtonClose);

    emit finished();
}
