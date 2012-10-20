#include "ModelInvoiceWithCommodities.h"

ModelInvoiceWithCommodities::ModelInvoiceWithCommodities(QObject *parent) :
    QSqlTableModel(parent)
{
    setTable("table_invoice_commodity");
}
