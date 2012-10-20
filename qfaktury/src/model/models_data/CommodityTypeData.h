#ifndef COMMODITYTYPEDATA_H
#define COMMODITYTYPEDATA_H

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
    enum CommodityType {GOODS = 1, SERVICES}; // "= 1" because SQL starts counting from 1

    /**
     * @brief
     *
     * @param type
     * @return QString
     */
    static QString names(const int type)
    {
        switch(type)
        {
        case GOODS: return QObject::trUtf8("Towar");
        case SERVICES: return QObject::trUtf8("Usługa");
        default:
            qDebug("CommodityTypeData::names(): Unknown commodity's type.");
            return QString();
        }
    }
};

#endif // COMMODITYTYPEDATA_H
