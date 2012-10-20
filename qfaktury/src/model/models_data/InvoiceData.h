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

namespace InvoiceDataFields
{
    /**
     * @brief
     *
     */
    enum Fields {ID, INV_NUMBER, SELLING_DATE, TYPE_ID, COUNTERPARTY_ID,
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
        case InvoiceDataFields::ID: return id;
        case InvoiceDataFields::INV_NUMBER: return invNumber;
        case InvoiceDataFields::SELLING_DATE: return sellingDate;
        case InvoiceDataFields::TYPE_ID: return typeID;
        case InvoiceDataFields::COUNTERPARTY_ID: return counterpartyID;
        case InvoiceDataFields::ISSUANCE_DATE: return issuanceDate;
        case InvoiceDataFields::PAYMENT_DATE: return paymentDate;
        case InvoiceDataFields::PAYMENT_ID: return paymentID;
        case InvoiceDataFields::CURRENCY_ID: return currencyID;
        case InvoiceDataFields::ADDIT_TEXT: return additText;
        case InvoiceDataFields::DISCOUNT: return discount;
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
