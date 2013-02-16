#ifndef MODELCOUNTERPARTY_H
#define MODELCOUNTERPARTY_H

#include <QSqlRelationalTableModel>
#include <QDebug>

#include "../models_data/CounterpartyData.h"

/**
 * @brief
 *
 */
class ModelCounterparty : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit ModelCounterparty(QObject *parent);
    /**
     * @brief
     *
     * @param section
     * @param orientation
     * @param role
     * @return QVariant
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    CounterpartyData myCompany() const;
};

#endif
