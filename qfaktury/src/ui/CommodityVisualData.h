#ifndef COMMODITYVISUALDATA_H
#define COMMODITYVISUALDATA_H

#include <QVariant>

namespace CommodityVisualFields
{
    /**
     * @brief
     *
     */
    enum Fields {ID, NAME, QUANTITY, UNIT, PKWIU, NET, VAT, TYPE, DISCOUNT};
}

/**
 * @brief
 *
 */
struct CommodityVisualData
{
    /**
     * @brief
     *
     * @param fieldnum
     * @return QString
     */
    QString field(const int fieldnum) const
    {
        switch(fieldnum)
        {
        case CommodityVisualFields::ID: return id;
        case CommodityVisualFields::NAME: return name;
        case CommodityVisualFields::QUANTITY: return quantity;
        case CommodityVisualFields::UNIT: return unit;
        case CommodityVisualFields::PKWIU: return pkwiu;
        case CommodityVisualFields::NET: return net;
        case CommodityVisualFields::VAT: return vat;
        case CommodityVisualFields::TYPE: return type;
        case CommodityVisualFields::DISCOUNT: return discount;
        default:
            qDebug("CommodityVisualData::field(): Unrecognized field number");
        }
        return QString();
    }

    /**
     * @brief
     *
     * @param fieldnum
     * @param v
     */
    void setField(const int fieldnum, const QVariant &v)
    {
        switch(fieldnum)
        {
        case CommodityVisualFields::ID:
            id = v.toString();
            break;
        case CommodityVisualFields::NAME:
            name = v.toString();
            break;
        case CommodityVisualFields::QUANTITY:
            quantity = v.toString();
            break;
        case CommodityVisualFields::UNIT:
            unit = v.toString();
            break;
        case CommodityVisualFields::PKWIU:
            pkwiu = v.toString();
            break;
        case CommodityVisualFields::NET:
            net = v.toString();
            break;
        case CommodityVisualFields::VAT:
            vat = v.toString();
            break;
        case CommodityVisualFields::TYPE:
            type = v.toString();
            break;
        case CommodityVisualFields::DISCOUNT:
            discount = v.toString();
            break;
        default:
            qDebug("CommodityVisualData::setField(): Unrecognized field number");
        }
    }

    /**
     * @brief
     *
     * @param fieldnum
     * @return QString
     */
    static QString header(const int fieldnum)
    {
        switch(fieldnum)
        {
        case CommodityVisualFields::ID: return QObject::trUtf8("ID");
        case CommodityVisualFields::NAME: return QObject::trUtf8("Nazwa");
        case CommodityVisualFields::QUANTITY: return QObject::trUtf8("Ilość");
        case CommodityVisualFields::UNIT: return QObject::trUtf8("Jednostka");
        case CommodityVisualFields::PKWIU: return QObject::trUtf8("PKWiU");
        case CommodityVisualFields::NET: return QObject::trUtf8("Netto");
        case CommodityVisualFields::VAT: return QObject::trUtf8("VAT");
        case CommodityVisualFields::TYPE: return QObject::trUtf8("Rodzaj");
        case CommodityVisualFields::DISCOUNT: return QObject::trUtf8("Zniżka");
        default:
            qDebug("CommodityVisualData::field(): Unrecognized field number");
        }
        return QString();
    }

    bool operator ==(const CommodityVisualData &cvd) const
    {
        for(int i = CommodityVisualFields::ID; i <= CommodityVisualFields::DISCOUNT; ++i)
        {
            if(this->field(i) != cvd.field(i))
            {
                return false;
            }
        }
        return true;
    }

    bool operator !=(const CommodityVisualData &cvd) const
    {
        return !(*this == cvd);
    }

    QString id; /**< TODO */
    QString name; /**< TODO */
    QString quantity; /**< TODO */
    QString unit; /**< TODO */
    QString pkwiu; /**< TODO */
    QString net; /**< TODO */
    QString vat; /**< TODO */
    QString type; /**< TODO */
    QString discount; /**< TODO */
};

Q_DECLARE_METATYPE(CommodityVisualData)
Q_DECLARE_METATYPE(QList<CommodityVisualData>)

#endif // COMMODITYVISUALDATA_H
