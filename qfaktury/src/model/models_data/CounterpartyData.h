/*
 * KontrData.h
 *
 *  Created on: Apr 18, 2009
 *      Author: moux
 */

#ifndef COUNTERPARTYDATA_H
#define COUNTERPARTYDATA_H

#include <QStringList>
#include <QVariant>

namespace CounterpartyFields
{
    /**
     * @brief
     *
     */
    enum Fields {ID, NAME, TYPE_ID, COUNTRY_ID, LOCATION, POSTAL_CODE, ADDRESS, TAX_ID, ACCOUNT_NAME, WWW, PRIMARY_EMAIL, PRIMARY_PHONE};
}


/**
 * @brief
 *
 */
class CounterpartyData
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
        switch(i)
        {
        case CounterpartyFields::ID:    return id;
        case CounterpartyFields::NAME: return name;
        case CounterpartyFields::TYPE_ID: return type_id;
        case CounterpartyFields::COUNTRY_ID: return country_id;
        case CounterpartyFields::LOCATION: return location;
        case CounterpartyFields::POSTAL_CODE: return postalCode;
        case CounterpartyFields::ADDRESS: return address;
        case CounterpartyFields::TAX_ID: return taxID;
        case CounterpartyFields::ACCOUNT_NAME: return accountName;
        case CounterpartyFields::WWW: return www;
        case CounterpartyFields::PRIMARY_EMAIL: return primary_email;
        case CounterpartyFields::PRIMARY_PHONE: return primary_phone;

        default:
            qDebug("CounterpartyData::field() - Unknown field");
            return QVariant();
        }
    }

public:
    QString id; /**< TODO */
    QString name; /**< TODO */
    int type_id; /**< TODO */
    int country_id; /**< TODO */
    QString location; /**< TODO */
    QString postalCode; /**< TODO */
    QString address;     /**< TODO */
    QString taxID; /**< TODO */
    QString accountName; /**< TODO */
    QString www; /**< TODO */
    QString primary_email; /**< TODO */
    QString primary_phone; /**< TODO */
};

#endif
