#ifndef GUIUSERADDNEWCOMMODITY_H
#define GUIUSERADDNEWCOMMODITY_H

#include "GuiUser.h"
#include "CommodityData.h"

struct CommodityData;
class DialogWithCommodityDialog;
class QComboBox;
class QDoubleSpinBox;

class GuiUserAddNewCommodity : public GuiUser
{
    Q_OBJECT
public:
    explicit GuiUserAddNewCommodity(DialogWithCommodityDialog *d, const CommodityData &commodity, QObject *parent = 0);
    ~GuiUserAddNewCommodity() {}
    
public slots:
    void process();

signals:
    void setComboBoxIndex(int);
    void setDoubleValue(double);

private:
    void postText_(QObject *obj, const QString &str);
    void postComboBoxIndex_(QComboBox *obj, const int index);
    void postDoubleVal_(QDoubleSpinBox *obj, const double val);

private:
    DialogWithCommodityDialog *dialog_;
    const CommodityData commodity_;
};

#endif // GUIUSERADDNEWCOMMODITY_H
