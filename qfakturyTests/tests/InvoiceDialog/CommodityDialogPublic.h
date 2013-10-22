#ifndef COMMODITYDIALOGPUBLIC_H
#define COMMODITYDIALOGPUBLIC_H

#include "CommodityDialog.h"


class CommodityDialogPublic : public CommodityDialog
{
    Q_OBJECT

public:
    CommodityDialogPublic(QWidget *parent, Database *db, const QModelIndex &id_edit = QModelIndex());
    Ui::CommodityDialog* ui();
};


#endif // COMMODITYDIALOGPUBLIC_H
