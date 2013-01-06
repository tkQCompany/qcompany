/*
 * InvoiceData.h
 *
 *  Created on: Mar 12, 2009
 *      Author: moux
 */

#ifndef INVOICEDATA_H
#define INVOICEDATA_H

#include "CommodityData.h"
#include "../../CustomPaymData.h"

namespace InvoiceFields
{
    /**
     * @brief
     *
     */
    enum Fields {ID_INVOICE, INV_NUMBER, SELLING_DATE, TYPE_ID, COUNTERPARTY_ID,
                 ISSUANCE_DATE, PAYMENT_DATE, PAYMENT_ID, CURRENCY_ID, ADDIT_TEXT, DISCOUNT};
}


/**
 * @brief
 *
 */
class InvoiceData
{
public:

    /**
     * @brief
     *
     * @param i
     * @return QVariant
     */
    QVariant field(const int i) const
    {
        QVariant var;
        switch(i)
        {
        case InvoiceFields::ID_INVOICE: return id;
        case InvoiceFields::INV_NUMBER: return invNumber;
        case InvoiceFields::SELLING_DATE: return sellingDate;
        case InvoiceFields::TYPE_ID: return typeID;
        case InvoiceFields::COUNTERPARTY_ID: return counterpartyID;
        case InvoiceFields::ISSUANCE_DATE: return issuanceDate;
        case InvoiceFields::PAYMENT_DATE: return paymentDate;
        case InvoiceFields::PAYMENT_ID: return paymentID;
        case InvoiceFields::CURRENCY_ID: return currencyID;
        case InvoiceFields::ADDIT_TEXT: return additText;
        case InvoiceFields::DISCOUNT: return discount;
        default:
            qDebug("Unknown field of 'InvoiceData' class");
            return var;
        }
    }
public:
    qint64 id; /**< TODO */
    QString invNumber; /**< TODO */
    QDate sellingDate; /**< TODO */
    qint64 typeID; /**< TODO */
    qint64 counterpartyID; /**< TODO */
    QDate issuanceDate; /**< TODO */
    QDate paymentDate; /**< TODO */
    qint64 paymentID; /**< TODO */
    qint64 currencyID; /**< TODO */
    QString additText; /**< TODO */
    short int discount; /**< TODO */

    //this field is not present in the above list of fields, because it's not for direct displaying
    QMap<int, CommodityData> products; /**< TODO */
};
#endif
