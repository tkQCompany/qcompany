#ifndef MODELINVOICE_H
#define MODELINVOICE_H

#include <QSqlRelationalTableModel>

#include "../models_data/InvoiceData.h"
#include "../models_data/InvoiceTypeData.h"
#include "../models_data/InvoiceNumberFormatData.h"

/**
 * @brief
 *
 */
class ModelInvoice : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit ModelInvoice(QObject *parent);
    /**
     * @brief
     *
     * @param section
     * @param orientation
     * @param role
     * @return QVariant
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    /**
     * @brief
     *
     * @return QString
     */
    QString generateInvoiceNumber(const QString &format, const QDate &currDate, const int invoiceType) const;
};

#endif // MODELINVOICE_H
