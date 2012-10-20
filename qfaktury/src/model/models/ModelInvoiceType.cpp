#include "ModelInvoiceType.h"

ModelInvoiceType::ModelInvoiceType(QObject *parent) :
    QSqlTableModel(parent, QSqlDatabase::database())
{
    setTable("invoice_type");
}
