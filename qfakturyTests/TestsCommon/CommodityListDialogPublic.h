#ifndef COMMODITYLISTDIALOGPUBLIC_H
#define COMMODITYLISTDIALOGPUBLIC_H

#include "CommodityListDialog.h"
#include "../../qfaktury/ui_files/ui_CommodityListDialog.h"

class CommodityListDialogPublic : public CommodityListDialog
{
    Q_OBJECT
public:
    explicit CommodityListDialogPublic(QWidget *parent, Database *db);
    Ui::CommodityListDialog* ui();
    virtual ~CommodityListDialogPublic() {}
    QDataWidgetMapper *getDataWidgetMapper() const;
};

#endif // COMMODITYLISTDIALOGPUBLIC_H
