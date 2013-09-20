#ifndef INVOICETYPEDATA_H
#define INVOICETYPEDATA_H

#include <QMetaType>

namespace InvoiceTypeFields
{
    /**
     * @brief
     *
     */
    enum Fields {ID_INVOICE_TYPE, INVOICE_TYPE};
}

/**
 * @brief
 *
 */
class InvoiceTypeData
{
public:
    /**
     * @brief
     *
     */
    enum Type {VAT, PRO_FORMA, CORRECTIVE_VAT, GROSS, CORRECTIVE_GROSS, BILL};

    /**
      *  Return invoice type
    */
    /**
     * @brief
     *
     * @param invoiceType
     * @return QString
     */
    static QString name(const Type invoiceType)
    {
        switch (invoiceType)
        {
        case VAT:               return QObject::trUtf8("Faktura VAT");
        case PRO_FORMA:         return QObject::trUtf8("Faktura Pro Forma");
        case CORRECTIVE_VAT:    return QObject::trUtf8("Korekta");
        case GROSS:             return QObject::trUtf8("Faktura brutto");
        case CORRECTIVE_GROSS:  return QObject::trUtf8("Korekta brutto");
        case BILL:              return QObject::trUtf8("Rachunek");
        default:
            qDebug("InvoiceTypeData::name(): Unknown type of invoice");
            return QString();
        }
    }

    /**
     * @brief
     *
     * @param invoiceType
     * @return QString
     */
    static QString shortName(const Type invoiceType)
    {
        switch (invoiceType)
        {
        case VAT:               return QObject::trUtf8("FVAT");
        case PRO_FORMA:         return QObject::trUtf8("FProForma");
        case CORRECTIVE_VAT:    return QObject::trUtf8("Korekta");
        case GROSS:             return QObject::trUtf8("Fbrutto");
        case CORRECTIVE_GROSS:  return QObject::trUtf8("KorektaBrutto");
        case BILL:              return QObject::trUtf8("Rachunek");
        default:
            qDebug("InvoiceTypeData::shortName(): Unknown type of invoice");
            return QString();
        }
    }

    static Type StringToInvoiceType(const QString& str)
    {
        Type ret = VAT;
        for(int i = VAT; i <= BILL; ++i)
        {
            if(0 == (str.compare(name((Type)i))))
            {
                ret = (Type)i;
                break;
            }
        }
        return ret;
    }
};

Q_DECLARE_METATYPE(InvoiceTypeData::Type)

#endif // INVOICETYPEDATA_H
