#include "ModelCounterpartyType.h"

ModelCounterpartyType::ModelCounterpartyType(QObject *parent) :
    QSqlTableModel(parent, QSqlDatabase::database())
{
    setTable("counterparty_type");
}
