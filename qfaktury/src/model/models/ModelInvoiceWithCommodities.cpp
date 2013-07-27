#include "ModelInvoiceWithCommodities.h"

ModelInvoiceWithCommodities::ModelInvoiceWithCommodities(QObject *parent) :
    QSqlRelationalTableModel(parent)
{
    setTable("table_invoice_commodity");
}
