#ifndef MODELCOUNTRY_H
#define MODELCOUNTRY_H

#include <QStringListModel>

#include "../models_data/CountryData.h"

/**
 * @brief
 *
 */
class ModelCountry : public QStringListModel
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
