#ifndef COMMODITYDATA_H
#define COMMODITYDATA_H

#include <QDebug>

#include "../../SettingsGlobal.h"

namespace CommodityFields
{
/**
 * @brief
 *
 */
enum Fields {ID, NAME, ABBREV, PKWIU, TYPE_ID, UNIT_ID, NET1, NET2, NET3, NET4, VAT, QUANTITY};
}


class CommodityData
{
public:
    CommodityData()
    {
        id = type_id = unit_id = net1 = net2 = net3 = net4 = quantity = 0;
    }

    QVariant field(const int i) const
    {
        switch(i)
        {
        case CommodityFields::ID: return id;
        case CommodityFields::NAME: return name;
        case CommodityFields::ABBREV: return abbrev;
        case CommodityFields::PKWIU: return pkwiu;
        case CommodityFields::TYPE_ID: return type_id;
        case CommodityFields::UNIT_ID: return unit_id;
        case CommodityFields::NET1: return net1;
        case CommodityFields::NET2: return net2;
        case CommodityFields::NET3: return net3;
        case CommodityFields::NET4: return net4;
        case CommodityFields::VAT: return vat;
        case CommodityFields::QUANTITY: return quantity;
        default:
            qDebug() << "CommodityData::field(): Unknown commodity field: " << i;
            return QVariant();
        }
    }


    void setField(const int i, const QVariant &v)
    {
        switch(i)
        {
        case CommodityFields::ID:
            id = v.toInt();
            break;
        case CommodityFields::NAME:
            name = v.toString();
            break;
        case CommodityFields::ABBREV:
            abbrev = v.toString();
            break;
        case CommodityFields::PKWIU:
            pkwiu = v.toString();
            break;
        case CommodityFields::TYPE_ID:
            type_id = v.toInt();
            break;
        case CommodityFields::UNIT_ID:
            unit_id = v.toInt();
            break;
        case CommodityFields::NET1:
            net1 = v.toDouble();
            break;
        case CommodityFields::NET2:
            net2 = v.toDouble();
            break;
        case CommodityFields::NET3:
            net3 = v.toDouble();
            break;
        case CommodityFields::NET4:
            net4 = v.toDouble();
            break;
        case CommodityFields::VAT:
            vat = v.toString();
            break;
        case CommodityFields::QUANTITY:
            quantity = v.toDouble();
            break;
        default:
            qDebug() << "CommodityData::setField(): Unknown commodity field: " << i;
        }
    }


    static QString header(const int i)
    {
        switch(i)
        {
        case CommodityFields::ID: return QObject::trUtf8("L.p.");
        case CommodityFields::NAME: return QObject::trUtf8("Nazwa");
        case CommodityFields::ABBREV: return QObject::trUtf8("Skrót nazwy");
        case CommodityFields::PKWIU: return QObject::trUtf8("PKWiU");
        case CommodityFields::TYPE_ID: return QObject::trUtf8("Typ");
        case CommodityFields::UNIT_ID: return QObject::trUtf8("Jednostka");
        case CommodityFields::NET1: return QObject::trUtf8("Netto 1");
        case CommodityFields::NET2: return QObject::trUtf8("Netto 2");
        case CommodityFields::NET3: return QObject::trUtf8("Netto 3");
        case CommodityFields::NET4: return QObject::trUtf8("Netto 4");
        case CommodityFields::VAT: return QObject::trUtf8("Stawka VAT");
        case CommodityFields::QUANTITY: return QObject::trUtf8("Ilość");
        default:
            qDebug() << "CommodityData::header(): Unknown commodity header: " << i;
            return QString();
        }
    }

public:
    quint64 id; /**< TODO */
    QString name; /**< TODO */
    QString abbrev; /**< TODO */
    QString pkwiu; /**< TODO */
    quint64 type_id; /**< TODO */
    quint64 unit_id; /**< TODO */
    double net1, net2, net3, net4; /**< TODO */
    QString vat; /**< TODO */
    double quantity; /**< TODO */
};

#endif
