#ifndef INVOICETYPEDATA_H
#define INVOICETYPEDATA_H

namespace InvoiceTypeFields
{
    /**
     * @brief
     *
     */
    enum Fields {ID_INVOICE_TYPE, TYPE};
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
    enum Type {VAT = 1, PRO_FORMA, CORRECTIVE_VAT, GROSS, CORRECTIVE_GROSS, BILL}; // "= 1" because SQL starts counting from 1

    /**
      *  Return invoice type
    */
    /**
     * @brief
     *
     * @param invoiceType
     * @return QString
     */
    static QString InvoiceTypeToString(const int invoiceType)
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
            qDebug("InvoiceTypeData::InvoiceTypeToString(): Unknown type of invoice");
            return QString();
        }
    }

    static int StringToInvoiceType(const QString& str)
    {
        int ret = -1;
        for(int i = VAT; i <= BILL; ++i)
        {
            if(0 == (str.compare(InvoiceTypeToString(i))))
            {
                ret = i;
                break;
            }
        }
        return ret;
    }
};

#endif // INVOICETYPEDATA_H
