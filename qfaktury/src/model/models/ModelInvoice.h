#ifndef MODELINVOICE_H
#define MODELINVOICE_H

#include <QSqlRelationalTableModel>

#include "../models_data/InvoiceData.h"

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
    int nextNumberInYear(const QDate &currDate) const;
    int nextNumberInMonth(const QDate &currDate) const;
    int nextNumberInDay(const QDate &currDate) const;
    int nextNumberInQuarter(const QDate &currDate) const;
};

#endif // MODELINVOICE_H
