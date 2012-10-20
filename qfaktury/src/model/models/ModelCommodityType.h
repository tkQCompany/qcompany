#ifndef MODELCOMMODITYTYPE_H
#define MODELCOMMODITYTYPE_H

#include <QSqlTableModel>

#include "../models_data/CommodityTypeData.h"

/**
 * @brief
 *
 */
class ModelCommodityType : public QSqlTableModel
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit ModelCommodityType(QObject *parent);
};

#endif // MODELCOMMODITYTYPE_H
