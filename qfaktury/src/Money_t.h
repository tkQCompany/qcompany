#ifndef MONEY_T_H
#define MONEY_T_H

#include <gmpxx.h>

#include "CurrencyData.h"

class Money_t;

inline bool operator==(const Money_t &lhs, const Money_t &rhs);
inline bool operator!=(const Money_t &lhs, const Money_t &rhs);
inline QDataStream& operator<<(QDataStream &ostr, const Money_t &money);
inline QDataStream& operator>>(QDataStream &istr, Money_t &money);

class Money_t
{
    friend bool operator==(const Money_t &lhs, const Money_t &rhs);
    friend bool operator!=(const Money_t &lhs, const Money_t &rhs);
    friend QDataStream& operator<<(QDataStream &ostr, const Money_t &money);
    friend QDataStream& operator>>(QDataStream &istr, Money_t &money);
    typedef mpq_class val_t;
public:
    explicit Money_t();
    explicit Money_t(const double val);
    explicit Money_t(const QString &val);

    void setValue(const double val);
    void setValue(const QString &val);

    Money_t& operator+=(const Money_t &rhs);
    Money_t  operator+ (const Money_t &rhs);
    Money_t& operator*=(const double rhs);
    Money_t  operator* (const double rhs);

    CurrencyData::Currencies currency() const {return currency_; }
    void setCurrency(CurrencyData::Currencies currency) {currency_ = currency; }

    QString verballyPL() const;
    QString toString() const;

private:
    CurrencyData::Currencies currency_;
    val_t value_;

private:
    short digit_(const val_t &num, const size_t index) const;
    void init_(const QString &val, const bool setDefaultCurrency);
    void setDefaultCurrency_();
    QString verballyPL1_999(const val_t &val) const;
};


inline bool operator==(const Money_t &lhs, const Money_t &rhs)
{
    return (lhs.value_ == rhs.value_) && (lhs.currency_ == rhs.currency_);
}

inline bool operator!=(const Money_t &lhs, const Money_t &rhs)
{
    return !(lhs == rhs);
}


inline QDataStream& operator<<(QDataStream &ostr, const Money_t &money)
{
    ostr << money.toString();
    return ostr;
}


inline QDataStream& operator>>(QDataStream &istr, Money_t &money)
{
    QString str;
    istr >> str;

    money = Money_t(str);
    return istr;
}

Q_DECLARE_METATYPE(Money_t)

#endif // MONEY_T_H
