#ifndef MONEY_T_H
#define MONEY_T_H

#include <gmpxx.h>

#include "CurrencyData.h"
#include "SettingsGlobal.h"

class Money_t
{
    typedef mpq_class val_t;
public:
    Money_t();
    Money_t(double val);
    Money_t(const QString &val);

    CurrencyData::Currencies getCurrency() const {return currency_; }
    void setCurrency(CurrencyData::Currencies currency) {currency_ = currency; }

    QString verballyPL() const;
    QString toString() const;

private:
    CurrencyData::Currencies currency_;
    val_t value_;

private:
    short digit_(const val_t &num, const size_t index) const;
    QString verballyPL1_999(const val_t &val) const;
};

Q_DECLARE_METATYPE(Money_t)

#endif // MONEY_T_H
