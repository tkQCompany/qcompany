#ifndef COMMODITYVISUALDATA_H
#define COMMODITYVISUALDATA_H

#include <QVariant>

#include "Money_t.h"

namespace CommodityVisualFields
{
    /**
     * @brief
     *
     */
    enum Field {ID, NAME, QUANTITY, UNIT, PKWIU, NET, VAT, TYPE, DISCOUNT};
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
    QVariant field(const CommodityVisualFields::Field fieldnum) const
    {
        QVariant val;
        switch(fieldnum)
        {
        case CommodityVisualFields::ID: return id;
        case CommodityVisualFields::NAME: return name;
        case CommodityVisualFields::QUANTITY: {val.setValue<DecVal>(quantity); return val;}
        case CommodityVisualFields::UNIT: return unit;
        case CommodityVisualFields::PKWIU: return pkwiu;
        case CommodityVisualFields::NET: {val.setValue<Money_t>(net); return val;}
        case CommodityVisualFields::VAT: {val.setValue<DecVal>(vat); return val;}
        case CommodityVisualFields::TYPE: return type;
        case CommodityVisualFields::DISCOUNT: {val.setValue<DecVal>(discount); return val;}
        default:
            qDebug("CommodityVisualData::field(): Unrecognized field number");
        }
        return val;
    }


    /**
     * @brief
     *
     * @param fieldnum
     * @param v
     */
    void setField(const CommodityVisualFields::Field fieldnum, const QVariant &v)
    {
        switch(fieldnum)
        {
        case CommodityVisualFields::ID:
            id = v.toLongLong();
            break;
        case CommodityVisualFields::NAME:
            name = v.toString();
            break;
        case CommodityVisualFields::QUANTITY:
            quantity = v.value<DecVal>();
            break;
        case CommodityVisualFields::UNIT:
            unit = v.toString();
            break;
        case CommodityVisualFields::PKWIU:
            pkwiu = v.toString();
            break;
        case CommodityVisualFields::NET:
            net = v.value<Money_t>();
            break;
        case CommodityVisualFields::VAT:
            vat = v.value<DecVal>();
            break;
        case CommodityVisualFields::TYPE:
            type = v.toString();
            break;
        case CommodityVisualFields::DISCOUNT:
            discount = v.value<DecVal>();
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
    static QString header(const CommodityVisualFields::Field fieldnum)
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
            if(this->field((CommodityVisualFields::Field)i) != cvd.field((CommodityVisualFields::Field)i))
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

    qlonglong id; /**< TODO */
    QString name; /**< TODO */
    DecVal quantity; /**< TODO */
    QString unit; /**< TODO */ //TODO: change type to appropriate
    QString pkwiu; /**< TODO */
    Money_t net; /**< TODO */
    DecVal vat; /**< TODO */
    QString type; /**< TODO */ //TODO: change type to appropriate
    DecVal discount; /**< TODO */
};

Q_DECLARE_METATYPE(CommodityVisualData)
Q_DECLARE_METATYPE(QList<CommodityVisualData>)

#endif // COMMODITYVISUALDATA_H
