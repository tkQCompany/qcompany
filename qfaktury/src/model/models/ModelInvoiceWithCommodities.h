#ifndef MODELINVOICEWITHCOMMODITIES_H
#define MODELINVOICEWITHCOMMODITIES_H

#include <QSqlRelationalTableModel>

/**
 * @brief
 *
 */
class ModelInvoiceWithCommodities : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit ModelInvoiceWithCommodities(QObject *parent);
};

#endif // MODELINVOICEWITHCOMMODITIES_H
