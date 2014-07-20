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
    void setID(const long long newID)
    {
        id_ = newID;
    }

    long long ID() const
    {
        return id_;
    }


    void setName(const QString &newName)
    {
        name_ = newName;
    }

    QString name() const
    {
        return name_;
    }


    void setQuantity(const DecVal &newQuantity)
    {
        quantity_ = newQuantity;
    }

    DecVal quantity() const
    {
        return quantity_;
    }


    void setUnit(const QString &newUnit)
    {
        unit_ = newUnit;
    }

    QString unit() const
    {
        return unit_;
    }


    void setPkwiu(const QString &newPkwiu)
    {
        pkwiu_ = newPkwiu;
    }

    QString pkwiu() const
    {
        return pkwiu_;
    }


    void setNet(const Money_t &newNetVal)
    {
        net_ = newNetVal;
    }

    Money_t net() const
    {
        return net_;
    }


    void setVat(const DecVal &newVat)
    {
        vat_ = newVat;
    }

    DecVal vat() const
    {
        return vat_;
    }


    void setType(const QString &newType)
    {
        type_ = newType;
    }

    QString type() const
    {
        return type_;
    }


    void setDiscount(const DecVal &newDiscount)
    {
        discount_ = newDiscount;
    }

    DecVal discount() const
    {
        return discount_;
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
        return (this->id_ == cvd.id_) &&
                (this->name_ == cvd.name_) &&
                (this->quantity_ == cvd.quantity_) &&
                (this->unit_ == cvd.unit_) &&
                (this->pkwiu_ == cvd.pkwiu_) &&
                (this->net_ == cvd.net_) &&
                (this->vat_ == cvd.vat_) &&
                (this->type_ == cvd.type_) &&
                (this->discount_ == cvd.discount_);
    }

    bool operator !=(const CommodityVisualData &cvd) const
    {
        return !(*this == cvd);
    }

private:
    qlonglong id_; /**< TODO */
    QString name_; /**< TODO */
    DecVal quantity_; /**< TODO */
    QString unit_; /**< TODO */ //TODO: change type to appropriate
    QString pkwiu_; /**< TODO */
    Money_t net_; /**< TODO */
    DecVal vat_; /**< TODO */
    QString type_; /**< TODO */ //TODO: change type to appropriate
    DecVal discount_; /**< TODO */
};

Q_DECLARE_METATYPE(CommodityVisualData)
Q_DECLARE_METATYPE(QList<CommodityVisualData>)

#endif // COMMODITYVISUALDATA_H
