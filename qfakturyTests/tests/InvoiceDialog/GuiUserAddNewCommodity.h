#ifndef GUIUSERADDNEWCOMMODITY_H
#define GUIUSERADDNEWCOMMODITY_H

#include "GuiUser.h"

class InvoiceDialogPublic;
struct CommodityVisualData;

class GuiUserAddNewCommodity : public GuiUser
{
    Q_OBJECT
public:
    explicit GuiUserAddNewCommodity(InvoiceDialogPublic *idp, const CommodityVisualData *commodity, QObject *parent = 0);
    
public slots:
    void process();

private:
    InvoiceDialogPublic *idp_;
    const CommodityVisualData *commodity_;
};

#endif // GUIUSERADDNEWCOMMODITY_H
