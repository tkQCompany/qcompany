#include <QSqlQuery>

#include "ModelCommodity.h"
#include "CommodityData.h"

ModelCommodity::ModelCommodity(QObject *parent) :
    QSqlRelationalTableModel(parent, QSqlDatabase::database())
{
    setTable("commodity");
}


double ModelCommodity::amount(const QString &id)
{
    QSqlQuery q(query());
    q.exec(QString("SELECT `quantity` FROM `commodity` WHERE `id_commodity`=%1").arg(id));
    if(q.next())
    {
        return q.value(0).toDouble(); //TODO: introduce qDecimal here
    }
    return 0.0;
}


QVariant ModelCommodity::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation != Qt::Horizontal)
        return QVariant();
    if(role != Qt::DisplayRole)
        return QVariant();

    return CommodityData::header(section);
}


bool ModelCommodity::changeAmount(const QString &id, const double change)
{
    double amountVal = amount(id);
    QSqlQuery q(query());
    if(q.exec(QString("UPDATE `commodity` SET `quantity` = %1 WHERE `id_commodity`=%2")
              .arg( ((amountVal + change) >=0)? (amountVal + change) : 0 ).arg(id)))
    {
        return true; //TODO: introduce qDecimal here
    }
    return false;
}
