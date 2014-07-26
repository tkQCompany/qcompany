#ifndef COUNTERPARTY_T_H
#define COUNTERPARTY_T_H

#include <QString>
#include <QMetaType>

struct Counterparty_t
{
public:
    long long id() const
    {
        return id_;
    }

    void setId( const long long id)
    {
        id_ = id;
    }


    QString name() const
    {
        return name_;
    }

    void setName(const QString &name)
    {
        name_ = name;
    }


    QString type() const
    {
        return type_;
    }

    void setType(const QString &type)
    {
        type_ = type;
    }


    QString country() const
    {
        return country_;
    }

    void setCountry(const QString &country)
    {
        country_ = country;
    }


    QString city() const
    {
        return city_;
    }

    void setCity(const QString &city)
    {
        city_ = city;
    }


    QString postalCode() const
    {
        return postalCode_;
    }

    void setPostalCode(const QString &postalCode)
    {
        postalCode_ = postalCode;
    }


    QString street() const
    {
        return street_;
    }

    void setStreet(const QString &street)
    {
        street_ = street;
    }


    QString taxIdent() const
    {
        return taxIdent_;
    }

    void setTaxIdent(const QString &taxIdent)
    {
        taxIdent_ = taxIdent;
    }


    QString accountNumber() const
    {
        return accountNumber_;
    }

    void setAccountNumber(const QString &accountNumber)
    {
        accountNumber_ = accountNumber;
    }


    QString www() const
    {
        return www_;
    }

    void setWww(const QString &www)
    {
        www_ = www;
    }


    QString email() const
    {
        return email_;
    }

    void setEmail(const QString &email)
    {
        email_ = email;
    }


    QString phone() const
    {
        return phone_;
    }

    void setPhone(const QString &phone)
    {
        phone_ = phone;
    }


    QString invNumFormat() const
    {
        return invNumFormat_;
    }

    void setInvNumFormat(const QString &invNumFormat)
    {
        invNumFormat_ = invNumFormat;
    }


private:
    long long id_;
    QString name_;
    QString type_;
    QString country_;
    QString city_;
    QString postalCode_;
    QString street_;
    QString taxIdent_;
    QString accountNumber_;
    QString www_;
    QString email_;
    QString phone_;
    QString invNumFormat_;
};


Q_DECLARE_METATYPE(Counterparty_t)

#endif // COUNTERPARTY_T_H
