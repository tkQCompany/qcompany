#ifndef GUIUSERADDCOMMODITY_H
#define GUIUSERADDCOMMODITY_H

#include "GuiUser.h"

class InvoiceDialogPublic;
struct CommodityVisualData;

class GuiUserAddCommodity : public GuiUser
{
    Q_OBJECT
public:
    explicit GuiUserAddCommodity(InvoiceDialogPublic *idp, const CommodityVisualData *commodity, QObject *parent = 0);
    
public slots:
    void process();

private:
    InvoiceDialogPublic *idp_;
    const CommodityVisualData *commodity_;
};

#endif // GUIUSERADDCOMMODITY_H
