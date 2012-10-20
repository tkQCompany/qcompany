#include "ModelPaymentType.h"

ModelPaymentType::ModelPaymentType(QObject *parent) :
    QSqlTableModel(parent, QSqlDatabase::database())
{
    setTable("payment_type");
}
