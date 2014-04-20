/*
 * InvoiceData.h
 *
 *  Created on: Mar 12, 2009
 *      Author: moux
 */

#ifndef INVOICEDATA_H
#define INVOICEDATA_H

#include <QDate>
#include <QVariant>

#include "Money_t.h"

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
    QVariant field(const InvoiceFields::Fields i) const
    {
        QVariant val;
        switch(i)
        {
        case InvoiceFields::ID_INVOICE:         return id_;
        case InvoiceFields::INV_NUMBER:         return invNumber_;
        case InvoiceFields::SELLING_DATE:       return sellingDate_;
        case InvoiceFields::TYPE_ID:            return typeID_;
        case InvoiceFields::COUNTERPARTY_ID:    return counterpartyID_;
        case InvoiceFields::ISSUANCE_DATE:      return issuanceDate_;
        case InvoiceFields::PAYMENT_DATE:       return paymentDate_;
        case InvoiceFields::PAYMENT_ID:         return paymentID_;
        case InvoiceFields::CURRENCY_ID:        return currencyID_;
        case InvoiceFields::ADDIT_TEXT:         return additText_;
        case InvoiceFields::DISCOUNT:           {val.setValue<Money_t::val_t>(discount_); return val;}
        default:
            qDebug("Unknown index in field(): (index=%d) detected in %s, line=%d.", i, __FILE__, __LINE__);
            return val;
        }
    }

    void setField(const InvoiceFields::Fields i, QVariant val)
    {
        switch(i)
        {
        case InvoiceFields::ID_INVOICE:
            id_ = val.toLongLong();
            break;
        case InvoiceFields::INV_NUMBER:
            invNumber_ = val.toString();
            break;
        case InvoiceFields::SELLING_DATE:
            sellingDate_ = val.toDate();
            break;
        case InvoiceFields::TYPE_ID:
            typeID_ = val.toLongLong();
            break;
        case InvoiceFields::COUNTERPARTY_ID:
            counterpartyID_ = val.toLongLong();
            break;
        case InvoiceFields::ISSUANCE_DATE:
            issuanceDate_ = val.toDate();
            break;
        case InvoiceFields::PAYMENT_DATE:
            paymentDate_ = val.toDate();
            break;
        case InvoiceFields::PAYMENT_ID:
            paymentID_ = val.toLongLong();
            break;
        case InvoiceFields::CURRENCY_ID:
            currencyID_ = val.toLongLong();
            break;
        case InvoiceFields::ADDIT_TEXT:
            additText_ = val.toString();
            break;
        case InvoiceFields::DISCOUNT:
            discount_ = val.value<Money_t::val_t>();
            break;
        default:
            qDebug("Unknown index in setField(): (index = %d, val=%s) detected in %s, line=%d.",
                   i, qPrintable(val.toString()), __FILE__, __LINE__);
        }
    }

    static QString header(const InvoiceFields::Fields i)
    {
        switch(i)
        {
        case InvoiceFields::ID_INVOICE:         return QObject::trUtf8("L.p.");
        case InvoiceFields::INV_NUMBER:         return QObject::trUtf8("Numer faktury");
        case InvoiceFields::SELLING_DATE:       return QObject::trUtf8("Data sprzedaży");
        case InvoiceFields::TYPE_ID:            return QObject::trUtf8("Rodzaj faktury");
        case InvoiceFields::COUNTERPARTY_ID:    return QObject::trUtf8("Nazwa kontrahenta");
        case InvoiceFields::ISSUANCE_DATE:      return QObject::trUtf8("Data wydania");
        case InvoiceFields::PAYMENT_DATE:       return QObject::trUtf8("Data płatności");
        case InvoiceFields::PAYMENT_ID:         return QObject::trUtf8("Rodzaj płatności");
        case InvoiceFields::CURRENCY_ID:        return QObject::trUtf8("Waluta");
        case InvoiceFields::ADDIT_TEXT:         return QObject::trUtf8("Tekst dodatkowy");
        case InvoiceFields::DISCOUNT:           return QObject::trUtf8("Zniżka");

        default:
            qDebug("Value:%d, file: %s, line: %d - Unknown header of invoices' table: %d", i, __FILE__, __LINE__, i);
            return QString();
        }
    }


private:
    qint64 id_; /**< TODO */
    QString invNumber_; /**< TODO */
    QDate sellingDate_; /**< TODO */
    qint64 typeID_; /**< TODO */
    qint64 counterpartyID_; /**< TODO */
    QDate issuanceDate_; /**< TODO */
    QDate paymentDate_; /**< TODO */
    qint64 paymentID_; /**< TODO */
    qint64 currencyID_; /**< TODO */
    QString additText_; /**< TODO */
    Money_t::val_t discount_; /**< TODO */
};
#endif
