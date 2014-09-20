/*
 * KontrData.h
 *
 *  Created on: Apr 18, 2009
 *      Author: moux
 */

#ifndef COUNTERPARTYDATA_H
#define COUNTERPARTYDATA_H

#include <QString>

#include "CounterpartyTypeData.h"

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
            qDebug("File: %s, line: %d - Unknown header of counterparties' table: %d", __FILE__, __LINE__, i);
            return QString();
        }
    }

    long long id() const;
    void setId(long long id);

    QString name() const;
    void setName(const QString &name);

    CounterpartyTypeData type_id() const;
    void setType_id(const CounterpartyTypeData &type_id);

    QString country() const;
    void setCountry(const QString &country);

    QString location() const;
    void setLocation(const QString &location);

    QString postal_code() const;
    void setPostal_code(const QString &postal_code);

    QString street() const;
    void setStreet(const QString &street);

    QString tax_ident() const;
    void setTax_ident(const QString &tax_ident);

    QString account_name() const;
    void setAccount_name(const QString &account_name);

    QString www() const;
    void setWww(const QString &www);

    QString primary_email() const;
    void setPrimary_email(const QString &primary_email);

    QString primary_phone() const;
    void setPrimary_phone(const QString &primary_phone);

    QString inv_num_format() const;
    void setInv_num_format(const QString &inv_num_format);

private:
    long long id_;
    QString name_;
    CounterpartyTypeData typeId_;
    QString country_;
    QString location_;
    QString postalCode_;
    QString street_;
    QString taxIdent_;
    QString accountName_;
    QString www_;
    QString primaryEmail_;
    QString primaryPhone_;
    QString invNumFormat_;
};

#endif
