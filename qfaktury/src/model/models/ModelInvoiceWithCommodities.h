#ifndef MODELINVOICEWITHCOMMODITIES_H
#define MODELINVOICEWITHCOMMODITIES_H

#include <QSqlTableModel>
#include <QSqlDatabase>

/**
 * @brief
 *
 */
class ModelInvoiceWithCommodities : public QSqlTableModel
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
