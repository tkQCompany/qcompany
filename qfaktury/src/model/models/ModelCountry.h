#ifndef MODELCOUNTRY_H
#define MODELCOUNTRY_H

#include <QSqlTableModel>

#include "../models_data/CountryData.h"

/**
 * @brief
 *
 */
class ModelCountry : public QSqlTableModel
{
    Q_OBJECT
public:
    /**
     * @brief
     *
     * @param parent
     */
    explicit ModelCountry(QObject *parent);
};

#endif // MODELCOUNTRY_H
