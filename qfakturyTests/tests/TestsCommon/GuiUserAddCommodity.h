#ifndef GUIUSERADDCOMMODITY_H
#define GUIUSERADDCOMMODITY_H

#include <QModelIndex>

#include "GuiUser.h"
#include "CommodityData.h"

class InvoiceDialogPublic;
struct CommodityData;
class QListView;
class QDoubleSpinBox;

class GuiUserAddCommodity : public GuiUser
{
    Q_OBJECT
public:
    explicit GuiUserAddCommodity(InvoiceDialogPublic *idp, const CommodityData &commodity, QObject *parent = 0);
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
    const InvoiceDialogPublic *idp_;
    const CommodityData commodity_;
};

#endif // GUIUSERADDCOMMODITY_H
