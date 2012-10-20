#ifndef MODELCOUNTERPARTYTYPE_H
#define MODELCOUNTERPARTYTYPE_H

#include <QSqlTableModel>

#include "../models_data/CounterpartyTypeData.h"

/**
 * @brief
 *
 */
class ModelCounterpartyType : public QSqlTableModel
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit ModelCounterpartyType(QObject *parent);
};

#endif // MODELCOUNTERPARTYTYPE_H
