#include <QSqlQuery>

#include "ModelCounterparty.h"
#include "CounterpartyData.h"
#include "CounterpartyTypeData.h"

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

    return CounterpartyData::header((CounterpartyFields::Field)section);
}


bool ModelCounterparty::isInvNumFormatEmpty(const QModelIndex &idCounterparty) const
{
    if(idCounterparty.isValid())
    {
        database().transaction();
        QSqlQuery query(this->query());
        query.exec(QString("SELECT inv_number_format FROM counterparty WHERE id_counterparty = %1").arg(idCounterparty.data().toLongLong()));
        if(query.isActive() && query.next())
        {
            if(!query.value(0).toString().isEmpty())
            {
                return false;

            }
        }
        database().commit();
    }
    return true;
}


void ModelCounterparty::setOnlyMyCompanyVisible(const bool yes)
{
    if(yes)
    {
        setFilter(QString("`type_id` = %1").arg(CounterpartyTypeData::MY_COMPANY));
    }
    else
    {
        setFilter(QString("`type_id` != %1").arg(CounterpartyTypeData::MY_COMPANY));
    }
    select();
}
