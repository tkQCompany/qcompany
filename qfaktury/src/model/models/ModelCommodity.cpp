#include <QSqlQuery>

#include "ModelCommodity.h"
#include "CommodityData.h"

ModelCommodity::ModelCommodity(QObject *parent) :
    QSqlRelationalTableModel(parent, QSqlDatabase::database())
{
    setTable("commodity");
}


Money_t::val_t ModelCommodity::amount(const qlonglong &id)
{
    QSqlQuery q(query());
    q.exec(QString("SELECT quantity FROM commodity WHERE id_commodity=%1").arg(id));
    if(q.next())
    {
        return Money_t::val_t(q.value(0).toDouble());
    }
    else
    {
        qDebug("ModelCommodity::amount(): couldn't successfully execute the SELECT SQL query");
    }
    return 0;
}


QVariant ModelCommodity::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation != Qt::Horizontal)
        return QVariant();
    if(role != Qt::DisplayRole)
        return QVariant();

    return CommodityData::header((CommodityFields::Field)section);
}


bool ModelCommodity::changeAmount(const qlonglong &id, const Money_t::val_t &change)
{
    Money_t::val_t amountVal = amount(id) + change;
    const Money_t::val_t newQuantity((amountVal >=0)? amountVal : Money_t::val_t(0));
    QSqlQuery q(query());
    if(q.exec(QString("UPDATE `commodity` SET `quantity` = %1 WHERE `id_commodity`=%2")
              .arg(newQuantity.get_str().c_str()).arg(id)))
    {
        return true;
    }
    return false;
}
