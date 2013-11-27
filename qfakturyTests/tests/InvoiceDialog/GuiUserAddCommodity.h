#ifndef GUIUSERADDCOMMODITY_H
#define GUIUSERADDCOMMODITY_H

#include "GuiUser.h"

class InvoiceDialogPublic;
struct CommodityData;

class GuiUserAddCommodity : public GuiUser
{
    Q_OBJECT
public:
    explicit GuiUserAddCommodity(InvoiceDialogPublic *idp, const CommodityData *commodity, QObject *parent = 0);
    
public slots:
    void process();

private:
    InvoiceDialogPublic *idp_;
    const CommodityData *commodity_;
};

#endif // GUIUSERADDCOMMODITY_H
