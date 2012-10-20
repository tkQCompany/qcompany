#ifndef MODELCURRENCY_H
#define MODELCURRENCY_H

#include <QSqlTableModel>

#include "../models_data/CurrencyData.h"

/**
 * @brief
 *
 */
class ModelCurrency : public QSqlTableModel
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit ModelCurrency(QObject *parent);
};

#endif // MODELCURRENCY_H
