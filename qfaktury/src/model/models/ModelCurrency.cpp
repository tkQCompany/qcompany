#include "ModelCurrency.h"

ModelCurrency::ModelCurrency(QObject *parent) :
    QSqlTableModel(parent, QSqlDatabase::database())
{
    setTable("currency");
}
