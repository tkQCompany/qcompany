#ifndef GUIUSERADDCOMMODITY_H
#define GUIUSERADDCOMMODITY_H

#include <QModelIndex>

#include "GuiUser.h"
#include "CommodityData.h"

struct CommodityData;
class DialogWithCommodityListDialog;
class QDoubleSpinBox;
class QListView;

class GuiUserAddCommodity : public GuiUser
{
    Q_OBJECT
public:
    explicit GuiUserAddCommodity(DialogWithCommodityListDialog *d, const CommodityData &commodity, QObject *parent = 0);
    ~GuiUserAddCommodity() {}
    
public slots:
    void process();

signals:
    void setCurrentIndex(const QModelIndex&);
    void setDoubleValue(double);

private:
    void postListViewIndex_(QListView *obj, const QModelIndex &index);
    void postDoubleVal_(QDoubleSpinBox *obj, const double val);

private:
    DialogWithCommodityListDialog *dialog_;
    const CommodityData commodity_;
};

#endif // GUIUSERADDCOMMODITY_H
