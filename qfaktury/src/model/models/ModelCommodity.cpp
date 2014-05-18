#include <QSqlQuery>

#include "ModelCommodity.h"
#include "CommodityData.h"

ModelCommodity::ModelCommodity(QObject *parent) :
    QSqlRelationalTableModel(parent, QSqlDatabase::database())
{
    setTable("commodity");
}


DecVal ModelCommodity::amount(const qlonglong &id)
{
    QSqlQuery q(query());
    q.exec(QString("SELECT quantity FROM commodity WHERE id_commodity=%1").arg(id));
    if(q.next())
    {
        return DecVal(q.value(0).toDouble());
    }
    else
    {
        qDebug("ModelCommodity::amount(): couldn't successfully execute the SELECT SQL query");
    }
    return DecVal(0.0);
}


QVariant ModelCommodity::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation != Qt::Horizontal)
        return QVariant();
    if(role != Qt::DisplayRole)
        return QVariant();

    return CommodityData::header((CommodityFields::Field)section);
}


bool ModelCommodity::changeAmount(const qlonglong &id, const DecVal &change)
{
    database().transaction();
    const DecVal amountVal = amount(id) + change;
    const DecVal decValZero(0.0);
    const DecVal newQuantity((amountVal >= decValZero)? amountVal : decValZero);
    QSqlQuery q(query());
    if(q.exec(QString("UPDATE `commodity` SET `quantity` = %1 WHERE `id_commodity`=%2")
              .arg(newQuantity.toString()).arg(id)))
    {
        return true;
    }
    database().commit();
    return false;
}
