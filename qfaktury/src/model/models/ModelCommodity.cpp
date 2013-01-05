#include "ModelCommodity.h"

ModelCommodity::ModelCommodity(QObject *parent) :
    QSqlRelationalTableModel(parent, QSqlDatabase::database())
{
    setTable("commodity");
}


QVariant ModelCommodity::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation != Qt::Horizontal)
        return QVariant();
    if(role != Qt::DisplayRole)
        return QVariant();

    return CommodityData::header(section);
}
