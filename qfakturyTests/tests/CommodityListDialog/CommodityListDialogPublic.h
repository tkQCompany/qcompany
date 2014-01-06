#ifndef COMMODITYLISTDIALOGPUBLIC_H
#define COMMODITYLISTDIALOGPUBLIC_H

#include "CommodityListDialog.h"

class CommodityListDialogPublic : public CommodityListDialog
{
    Q_OBJECT
public:
    explicit CommodityListDialogPublic(QWidget *parent, Database *db);
    Ui::CommodityListDialog* ui();
    virtual ~CommodityListDialogPublic() {}
};

#endif // COMMODITYLISTDIALOGPUBLIC_H