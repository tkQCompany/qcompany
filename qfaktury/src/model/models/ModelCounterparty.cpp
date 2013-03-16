#include "ModelCounterparty.h"

ModelCounterparty::ModelCounterparty(QObject *parent) :
    QSqlRelationalTableModel(parent, QSqlDatabase::database())
{
    setTable("counterparty");
}


QVariant ModelCounterparty::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Vertical)
        return QVariant();

    if(role != Qt::DisplayRole)
        return QVariant();

    return CounterpartyData::header(section);
}


void ModelCounterparty::setOnlyMyCompanyVisible(const bool yes)
{
    if(yes)
    {
        setFilter(QString("`type_id` = %1").arg(CounterpartyTypeData::MY_COMPANY + 1));
    }
    else
    {
        setFilter(QString("`type_id` != %1").arg(CounterpartyTypeData::MY_COMPANY + 1));
    }
    select();
}
