#include "ModelCounterpartyType.h"

ModelCounterpartyType::ModelCounterpartyType(QObject *parent) :
    QSqlTableModel(parent, QSqlDatabase::database())
{
    setTable("counterparty_type");
}

void ModelCounterpartyType::setMyCompanyVisible(const bool yes,
                                                   const bool onlyMyCompany)
{
    if(yes)
    {
        if(onlyMyCompany)
        {
            setFilter(QString("id_counterparty_type = %1")
                      .arg(CounterpartyTypeData::MY_COMPANY + 1));
        }
        else
        {
            setFilter("");
        }
    }
    else
    {
        setFilter(QString("id_counterparty_type != %1")
                  .arg(CounterpartyTypeData::MY_COMPANY + 1));
    }
    select();
}
