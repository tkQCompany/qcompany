#ifndef GUIUSEREDITINVOICE_H
#define GUIUSEREDITINVOICE_H

#include "GuiUser.h"
#include "InvoiceData.h"
#include "CommodityVisualData.h"


class MainWindowPublic;


class GuiUserEditInvoice : public GuiUser
{
    Q_OBJECT
public:
    explicit GuiUserEditInvoice(MainWindowPublic *mwp, QObject *parent = 0);
    const InvoiceData invoiceData() const { return invData_; }
    const QList<CommodityVisualData> commodities() const { return commodities_; }
    const Money_t totalNetVal() const { return totalNetVal_; }
    const Money_t totalDiscountVal() const { return totalDiscountVal_; }
    const Money_t totalGrossVal() const { return totalGrossVal_; }
    
signals:
    void finished();
    
public slots:
    void process();

private:
    MainWindowPublic *mwp_;
    InvoiceData invData_;
    QList<CommodityVisualData> commodities_;
    Money_t totalNetVal_, totalDiscountVal_, totalGrossVal_;
};

#endif // GUIUSEREDITINVOICE_H
