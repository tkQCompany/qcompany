#ifndef MODELUNIT_H
#define MODELUNIT_H

#include <QSqlTableModel>

#include "../models_data/UnitData.h"

/**
 * @brief
 *
 */
class ModelUnit : public QSqlTableModel
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit ModelUnit(QObject *parent);
};

#endif // MODELUNIT_H
