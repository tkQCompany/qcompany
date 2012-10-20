#include "ModelCommodityType.h"

ModelCommodityType::ModelCommodityType(QObject *parent) :
    QSqlTableModel(parent, QSqlDatabase::database())
{
    setTable("commodity_type");
}
