#ifndef MODELINVOICETYPE_H
#define MODELINVOICETYPE_H

#include <QSqlTableModel>

/**
 * @brief
 *
 */
class ModelInvoiceType : public QSqlTableModel
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit ModelInvoiceType(QObject *parent = 0);
};

#endif // MODELINVOICETYPE_H
