#ifndef DIALOGWITHCOMMODITYDIALOG_H
#define DIALOGWITHCOMMODITYDIALOG_H

#include "DialogWithPublicDialog.h"

class CommodityDialogPublic;

class DialogWithCommodityDialog : public DialogWithPublicDialog
{
public:
    virtual CommodityDialogPublic* commodityDialogPublic() const = 0;
};

#endif // DIALOGWITHCOMMODITYDIALOG_H
