#ifndef MODELPHONE_H
#define MODELPHONE_H

#include <QSqlRelationalTableModel>

#include "../models_data/PhoneData.h"

/**
 * @brief
 *
 */
class ModelPhone : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit ModelPhone(QObject *parent);
    /**
     * @brief
     *
     * @param idCounterparty
     */
    void setIDCounterparty(const QString &idCounterparty);
};

#endif // MODELPHONE_H
