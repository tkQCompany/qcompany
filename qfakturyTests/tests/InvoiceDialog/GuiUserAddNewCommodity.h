#ifndef GUIUSERADDNEWCOMMODITY_H
#define GUIUSERADDNEWCOMMODITY_H

#include "GuiUser.h"

class InvoiceDialogPublic;
struct CommodityData;

class GuiUserAddNewCommodity : public GuiUser
{
    Q_OBJECT
public:
    explicit GuiUserAddNewCommodity(InvoiceDialogPublic *idp, const CommodityData *commodity, QObject *parent = 0);
    
public slots:
    void process();

private:
    InvoiceDialogPublic *idp_;
    const CommodityData *commodity_;
};

#endif // GUIUSERADDNEWCOMMODITY_H
