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
