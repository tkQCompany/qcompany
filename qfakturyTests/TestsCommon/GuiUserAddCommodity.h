#ifndef GUIUSERADDCOMMODITY_H
#define GUIUSERADDCOMMODITY_H

#include <QModelIndex>

#include "GuiUser.h"
#include "CommodityData.h"

struct CommodityData;
class DialogWithCommodityListDialog;
class QComboBox;
class QSpinBox;
class QDoubleSpinBox;
class QListView;

class GuiUserAddCommodity : public GuiUser
{
    Q_OBJECT
public:
    explicit GuiUserAddCommodity(DialogWithCommodityListDialog *d, const CommodityData &commodity, const int whichNetVal, const DecVal &discount, QObject *parent = 0);
    ~GuiUserAddCommodity() {}
    
public slots:
    void process();

signals:
    void setComboBoxIndex(int);
    void setCurrentIndex(const QModelIndex&);
    void setDoubleValue(double);
    void setSpinBoxValue(int);

private:
    void postComboBoxIndex_(const int index);
    void postListViewIndex_(const QModelIndex &index);
    void postDoubleVal_(const double val);
    void postSpinBoxVal_(const DecVal &val);

private:
    DialogWithCommodityListDialog *dialog_;
    const CommodityData commodity_;
    const int netValIndex_;
    const DecVal discount_;
};

#endif // GUIUSERADDCOMMODITY_H
