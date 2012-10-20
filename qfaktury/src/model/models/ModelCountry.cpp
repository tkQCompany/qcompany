#include "ModelCountry.h"

ModelCountry::ModelCountry(QObject *parent) :
    QSqlTableModel(parent, QSqlDatabase::database())
{
    setTable("country");
}
