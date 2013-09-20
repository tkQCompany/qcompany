#ifndef COMMODITYDELEGATE_H
#define COMMODITYDELEGATE_H

#include <QSqlRelationalDelegate>

class CommodityDelegate : public QSqlRelationalDelegate
{
    Q_OBJECT
public:
    explicit CommodityDelegate(QObject *parent = 0);

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

#endif // COMMODITYDELEGATE_H
