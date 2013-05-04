/*
 * KontrData.h
 *
 *  Created on: Apr 18, 2009
 *      Author: moux
 */

#include <QDebug>

#ifndef COUNTERPARTYDATA_H
#define COUNTERPARTYDATA_H

namespace CounterpartyFields
{
/**
     * @brief
     *
     */
enum Field {ID, NAME, TYPE_ID, COUNTRY, LOCATION, POSTAL_CODE, STREET, TAX_IDENT, ACCOUNT_NAME, WWW, PRIMARY_EMAIL, PRIMARY_PHONE, INV_NUM_FORMAT};
}



class CounterpartyData
{
public:
    static QString header(const CounterpartyFields::Field i)
    {
        switch(i)
        {
        case CounterpartyFields::ID: return QObject::trUtf8("L.p.");
        case CounterpartyFields::NAME: return QObject::trUtf8("Nazwa");
        case CounterpartyFields::TYPE_ID: return QObject::trUtf8("Firma/Urząd");
        case CounterpartyFields::COUNTRY: return QObject::trUtf8("Kraj");
        case CounterpartyFields::LOCATION: return QObject::trUtf8("Miejscowość");
        case CounterpartyFields::POSTAL_CODE: return QObject::trUtf8("Kod pocztowy");
        case CounterpartyFields::STREET: return QObject::trUtf8("Ulica");
        case CounterpartyFields::TAX_IDENT: return QObject::trUtf8("NIP");
        case CounterpartyFields::ACCOUNT_NAME: return QObject::trUtf8("Numer konta");
        case CounterpartyFields::WWW: return QObject::trUtf8("WWW");
        case CounterpartyFields::PRIMARY_EMAIL: return QObject::trUtf8("Główny e-mail");
        case CounterpartyFields::PRIMARY_PHONE: return QObject::trUtf8("Główny telefon");
        case CounterpartyFields::INV_NUM_FORMAT: return QObject::trUtf8("Format numeru faktury");

        default:
            qDebug() << QString("File: %1, line: %2 - Unknown header of counterparties' table: %3").arg(__FILE__).arg(__LINE__).arg(i);
            return QString();
        }
    }
};


#endif
