#ifndef COMMODITYDATA_H
#define COMMODITYDATA_H

#include <QDebug>

#include "SettingsGlobal.h"

namespace CommodityFields
{
/**
 * @brief
 *
 */
enum Field {ID_COMMODITY, NAME, ABBREV, PKWIU, TYPE_ID, UNIT_ID, NET1, NET2, NET3, NET4, VAT, QUANTITY};
}


class CommodityData
{
public:
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
            qDebug() << "CommodityData::header(): Unknown commodity header: " << i;
            return QString();
        }
    }
};

#endif
