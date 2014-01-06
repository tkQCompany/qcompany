#ifndef COMMODITYDATA_H
#define COMMODITYDATA_H

#include "SettingsGlobal.h"
#include "Money_t.h"

namespace CommodityFields
{
/**
 * @brief
 *
 */
enum Field {ID_COMMODITY, NAME, ABBREV, PKWIU, TYPE_ID, UNIT_ID, NET1, NET2, NET3, NET4, VAT, QUANTITY};
}


struct CommodityData
{
public:
    CommodityData(): id_(0), type_id_(0), unit_id_(0), vat_(0.0f), quantity_(0.0) {}

    static QString header(const CommodityFields::Field i)
    {
        switch(i)
        {
        case CommodityFields::ID_COMMODITY: return QObject::trUtf8("L.p.");
        case CommodityFields::NAME: return QObject::trUtf8("Nazwa");
        case CommodityFields::ABBREV: return QObject::trUtf8("Skrót nazwy");
        case CommodityFields::PKWIU: return QObject::trUtf8("PKWiU");
        case CommodityFields::TYPE_ID: return QObject::trUtf8("Towar/Usługa");
        case CommodityFields::UNIT_ID: return QObject::trUtf8("Jednostka");
        case CommodityFields::NET1: return QObject::trUtf8("Netto 1");
        case CommodityFields::NET2: return QObject::trUtf8("Netto 2");
        case CommodityFields::NET3: return QObject::trUtf8("Netto 3");
        case CommodityFields::NET4: return QObject::trUtf8("Netto 4");
        case CommodityFields::VAT: return QObject::trUtf8("VAT");
        case CommodityFields::QUANTITY: return QObject::trUtf8("Ilość");
        default:
            qDebug("CommodityData::header(): Unknown commodity header: %d", (int)i);
            return QString();
        }
    }

    QVariant field(const CommodityFields::Field i) const
    {
        switch(i)
        {
        case CommodityFields::ID_COMMODITY: return id_;
        case CommodityFields::NAME: return name_;
        case CommodityFields::ABBREV: return abbrev_;
        case CommodityFields::PKWIU: return pkwiu_;
        case CommodityFields::TYPE_ID: return type_id_;
        case CommodityFields::UNIT_ID: return unit_id_;
        case CommodityFields::NET1: return net1_.toString();
        case CommodityFields::NET2: return net2_.toString();
        case CommodityFields::NET3: return net3_.toString();
        case CommodityFields::NET4: return net4_.toString();
        case CommodityFields::VAT: return vat_;
        case CommodityFields::QUANTITY: return quantity_;
        default:
            qDebug("CommodityData::field(): Unknown commodity field: %d", (int)i);
            return QVariant();
        }
    }

    void setField(const CommodityFields::Field i, QVariant val)
    {
        switch(i)
        {
        case CommodityFields::ID_COMMODITY:
            id_ = val.toLongLong();
            break;
        case CommodityFields::NAME:
            name_ = val.toString();
            break;
        case CommodityFields::ABBREV:
            abbrev_ = val.toString();
            break;;
        case CommodityFields::PKWIU:
            pkwiu_ = val.toString();
            break;
        case CommodityFields::TYPE_ID:
            type_id_ = val.toInt(); //int, because comboboxes in qt operate on ints
            break;
        case CommodityFields::UNIT_ID:
            unit_id_ = val.toInt(); //int, because comboboxes in qt operate on ints
            break;
        case CommodityFields::NET1:
            net1_ = val.value<Money_t>();
            break;
        case CommodityFields::NET2:
            net2_ = val.value<Money_t>();
            break;
        case CommodityFields::NET3:
            net3_ = val.value<Money_t>();
            break;
        case CommodityFields::NET4:
            net4_ = val.value<Money_t>();
            break;
        case CommodityFields::VAT:
            vat_ = val.toFloat();
            break;
        case CommodityFields::QUANTITY:
            quantity_ = val.toDouble();
            break;
        default:
            qDebug("Unknown index in CommodityData::setField(): (index = %d, val=%s) detected in %s, line=%d.",
                   i, qPrintable(val.toString()), __FILE__, __LINE__);
        }
    }

private:
    qint64 id_;
    QString name_;
    QString abbrev_;
    QString pkwiu_;
    int type_id_;
    int unit_id_;
    Money_t net1_;
    Money_t net2_;
    Money_t net3_;
    Money_t net4_;
    float vat_;
    double quantity_;
};

Q_DECLARE_METATYPE(CommodityData)

#endif
