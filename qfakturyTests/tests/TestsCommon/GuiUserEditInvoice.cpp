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

    postMouseClick(idp->ui()->pushButtonClose);

    emit finished();
}
