#include "ModelCommodity.h"

ModelCommodity::ModelCommodity(QObject *parent) :
    QSqlRelationalTableModel(parent, QSqlDatabase::database())
{
    setTable("commodity");
}


QVariant ModelCommodity::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation != Qt::Horizontal)
        return QVariant();
    if(role != Qt::DisplayRole)
        return QVariant();
    switch(section)
    {
    case CommodityFields::ID:
        return trUtf8("L.p.");
    case CommodityFields::NAME:
        return trUtf8("Nazwa");
    case CommodityFields::ABBREV:
        return trUtf8("Skrót");
    case CommodityFields::PKWIU:
        return trUtf8("PKWiU");
    case CommodityFields::TYPE_ID:
        return trUtf8("Towar/Usługa");
    case CommodityFields::UNIT_ID:
        return trUtf8("Jednostka");
    case CommodityFields::NET1:
        return trUtf8("Netto1");
    case CommodityFields::NET2:
        return trUtf8("Netto2");
    case CommodityFields::NET3:
        return trUtf8("Netto3");
    case CommodityFields::NET4:
        return trUtf8("Netto4");
    case CommodityFields::VAT:
        return trUtf8("VAT");
    case CommodityFields::QUANTITY:
        return trUtf8("Ilość");
    default:
        qDebug() << QString("File: %1, line: %2 - Unknown header of commodities' table: %3").arg(__FILE__).arg(__LINE__).arg(section);
        return QVariant();
    }
}
