#ifndef COMMODITYDATA_H
#define COMMODITYDATA_H

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
    CommodityData(): id_(0), typeId_(0), unitId_(0) {}

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


    long long id() const
    {
        return id_;
    }

    void setId(const long long id)
    {
        id_ = id;
    }


    QString name() const
    {
        return name_;
    }

    void setName(const QString &name)
    {
        name_ = name;
    }


    QString abbrev() const
    {
        return abbrev_;
    }

    void setAbbrev(const QString &abbrev)
    {
        abbrev_ = abbrev;
    }


    QString pkwiu() const
    {
        return pkwiu_;
    }

    void setPkwiu(const QString &pkwiu)
    {
        pkwiu_ = pkwiu;
    }


    long long typeId() const
    {
        return typeId_;
    }

    void setTypeId(const long long typeId)
    {
        typeId_ = typeId;
    }


    long long unitId() const
    {
        return unitId_;
    }

    void setUnitId(const long long unitId)
    {
        unitId_ = unitId;
    }


    Money_t net1() const
    {
        return net1_;
    }

    void setNet1(const Money_t &net1)
    {
        net1_ = net1;
    }


    Money_t net2() const
    {
        return net2_;
    }

    void setNet2(const Money_t &net2)
    {
        net2_ = net2;
    }


    Money_t net3() const
    {
        return net3_;
    }

    void setNet3(const Money_t &net3)
    {
        net3_ = net3;
    }


    Money_t net4() const
    {
        return net4_;
    }

    void setNet4(const Money_t &net4)
    {
        net4_ = net4;
    }


    DecVal vat() const
    {
        return vat_;
    }

    void setVat(const DecVal &vat)
    {
        vat_ = vat;
    }


    DecVal quantity() const
    {
        return quantity_;
    }

    void setQuantity(const DecVal &quantity)
    {
        quantity_ = quantity;
    }

private:
    long long id_;
    QString name_;
    QString abbrev_;
    QString pkwiu_;
    long long typeId_;
    long long unitId_;
    Money_t net1_;
    Money_t net2_;
    Money_t net3_;
    Money_t net4_;
    DecVal vat_;
    DecVal quantity_;
};

Q_DECLARE_METATYPE(CommodityData)
Q_DECLARE_METATYPE(QList<CommodityData>)

#endif
