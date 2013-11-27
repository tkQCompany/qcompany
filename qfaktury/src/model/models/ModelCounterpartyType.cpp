#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

#include "ModelCounterpartyType.h"
#include "CounterpartyTypeData.h"

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


bool ModelCounterpartyType::addType(const QString &type)
{
    return addDelType_(type, true);
}

bool ModelCounterpartyType::delType(const QString &type)
{
    return addDelType_(type, false);
}


bool ModelCounterpartyType::addDelType_(const QString &type, const bool addType)
{
    QSqlQuery q(query());
    database().transaction();
    if(q.exec(QString("SELECT `id_counterparty_type` FROM counterparty_type WHERE `type` = '%1'")
              .arg(type)))
    {
        const QString queryStr(addType?
                                "INSERT INTO counterparty_type(`type`) VALUES('%1')":
                                "DELETE FROM counterparty_type WHERE `type` = '%1'");
        if((q.next() ^ addType) && q.exec(queryStr.arg(type)))
        {
            database().commit();
            select();
            return true;
        }
    }
    qDebug() << QString("ModelCounterpartyType::%1: ")
                .arg(addType? "addType()" : "delType()") << q.lastError().text();
    database().rollback();
    return false;
}
