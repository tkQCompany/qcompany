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
    void setId(const long long newId)
    {
        id_ = newId;
    }

    long long id() const
    {
        return id_;
    }


    void setInvNumber(const QString &newInvNumber)
    {
        invNumber_ = newInvNumber;
    }

    QString invNumber() const
    {
        return invNumber_;
    }


    void setSellingDate(const QDate &newDate)
    {
        sellingDate_ = newDate;
    }

    QDate sellingDate() const
    {
        return sellingDate_;
    }


    void setTypeID(const long long typeID)
    {
        typeID_ = typeID;
    }

    long long typeID() const
    {
        return typeID_;
    }


    void setCounterpartyID(const long long counterpartyID)
    {
        counterpartyID_ = counterpartyID;
    }

    long long counterpartyID() const
    {
        return counterpartyID_;
    }


    void setIssuanceDate(const QDate &issuanceDate)
    {
        issuanceDate_ = issuanceDate;
    }

    QDate issuanceDate() const
    {
        return issuanceDate_;
    }


    void setPaymentDate(const QDate &paymentDate)
    {
        paymentDate_ = paymentDate;
    }

    QDate paymentDate() const
    {
        return paymentDate_;
    }


    void setPaymentID(const long long paymentID)
    {
        paymentID_ = paymentID;
    }

    long long paymentID() const
    {
        return paymentID_;
    }


    void setCurrencyID(const long long currencyID)
    {
        currencyID_ = currencyID;
    }

    long long currencyID() const
    {
        return currencyID_;
    }


    void setAdditText(const QString &additText)
    {
        additText_ = additText;
    }

    QString additText() const
    {
        return additText_;
    }


    void setDiscount(const DecVal &discount)
    {
        discount_ = discount;
    }

    DecVal discount() const
    {
        return discount_;
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
    long long id_; /**< TODO */
    QString invNumber_; /**< TODO */
    QDate sellingDate_; /**< TODO */
    long long typeID_; /**< TODO */
    long long counterpartyID_; /**< TODO */
    QDate issuanceDate_; /**< TODO */
    QDate paymentDate_; /**< TODO */
    long long paymentID_; /**< TODO */
    long long currencyID_; /**< TODO */
    QString additText_; /**< TODO */
    DecVal discount_; /**< TODO */
};
#endif






