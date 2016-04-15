#include "CounterpartyData.h"

long long CounterpartyData::id() const
{
    return id_;
}

void CounterpartyData::setId(long long id)
{
    id_ = id;
}
QString CounterpartyData::name() const
{
    return name_;
}

void CounterpartyData::setName(const QString &name)
{
    name_ = name;
}
CounterpartyTypeData CounterpartyData::type_id() const
{
    return typeId_;
}

void CounterpartyData::setType_id(const CounterpartyTypeData &type_id)
{
    typeId_ = type_id;
}
QString CounterpartyData::country() const
{
    return country_;
}

void CounterpartyData::setCountry(const QString &country)
{
    country_ = country;
}
QString CounterpartyData::location() const
{
    return location_;
}

void CounterpartyData::setLocation(const QString &location)
{
    location_ = location;
}
QString CounterpartyData::postal_code() const
{
    return postalCode_;
}

void CounterpartyData::setPostal_code(const QString &postal_code)
{
    postalCode_ = postal_code;
}
QString CounterpartyData::street() const
{
    return street_;
}

void CounterpartyData::setStreet(const QString &street)
{
    street_ = street;
}
QString CounterpartyData::tax_ident() const
{
    return taxIdent_;
}

void CounterpartyData::setTax_ident(const QString &tax_ident)
{
    taxIdent_ = tax_ident;
}
QString CounterpartyData::account_name() const
{
    return accountName_;
}

void CounterpartyData::setAccountName(const QString &account_name)
{
    accountName_ = account_name;
}
QString CounterpartyData::www() const
{
    return www_;
}

void CounterpartyData::setWww(const QString &www)
{
    www_ = www;
}
QString CounterpartyData::primary_email() const
{
    return primaryEmail_;
}

void CounterpartyData::setPrimary_email(const QString &primary_email)
{
    primaryEmail_ = primary_email;
}
QString CounterpartyData::primary_phone() const
{
    return primaryPhone_;
}

void CounterpartyData::setPrimary_phone(const QString &primary_phone)
{
    primaryPhone_ = primary_phone;
}
QString CounterpartyData::inv_num_format() const
{
    return invNumFormat_;
}

void CounterpartyData::setInv_num_format(const QString &inv_num_format)
{
    invNumFormat_ = inv_num_format;
}
