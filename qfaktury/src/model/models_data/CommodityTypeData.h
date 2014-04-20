#ifndef COMMODITYTYPEDATA_H
#define COMMODITYTYPEDATA_H

#include <QString>

namespace CommodityTypeFields
{
/**
 * @brief
 *
 */
enum Fields {ID_COMMODITY_TYPE, TYPE};
}

/**
 * @brief
 *
 */
class CommodityTypeData
{
public:
    /**
     * @brief
     *
     */
    enum CommodityType {GOODS, SERVICES};

    /**
     * @brief
     *
     * @param type
     * @return QString
     */
    static QString name(const CommodityType type)
    {
        switch(type)
        {
        case GOODS: return QObject::trUtf8("Towar");
        case SERVICES: return QObject::trUtf8("Usługa");
        default:
            qDebug("CommodityTypeData::name(): Unknown commodity's type.");
            return QString();
        }
    }
};

#endif // COMMODITYTYPEDATA_H
