#ifndef DIALOGWITHCOMMODITYLISTDIALOG_H
#define DIALOGWITHCOMMODITYLISTDIALOG_H

#include "DialogWithPublicDialog.h"

class CommodityListDialogPublic;

class DialogWithCommodityListDialog : public DialogWithPublicDialog
{
public:
    virtual CommodityListDialogPublic* commodityListDialogPublic() const = 0;
};

#endif // DIALOGWITHCOMMODITYLISTDIALOG_H
