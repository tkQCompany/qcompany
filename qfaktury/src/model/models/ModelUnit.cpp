#include "ModelUnit.h"

ModelUnit::ModelUnit(QObject *parent) :
    QSqlTableModel(parent, QSqlDatabase::database())
{
    setTable("unit");
}
