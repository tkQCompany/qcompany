#include "ModelEmail.h"

ModelEmail::ModelEmail(QObject *parent, const QString &id_counterparty) :
    QSqlRelationalTableModel(parent, QSqlDatabase::database())
{
    setTable("additional_email");
    setFilter(QString("`id_counterparty` = %1").arg(id_counterparty));
}
