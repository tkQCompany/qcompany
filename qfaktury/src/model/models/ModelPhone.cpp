#include "ModelPhone.h"

#include <QDebug>

ModelPhone::ModelPhone(QObject *parent) :
    QSqlRelationalTableModel(parent, QSqlDatabase::database())
{
    setTable("additional_phone");
}


void ModelPhone::setIDCounterparty(const QString &idCounterparty)
{
    setFilter(QString("`id_counterparty` = %1").arg(idCounterparty));
}
