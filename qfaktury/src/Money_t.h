#ifndef MONEY_T_H
#define MONEY_T_H

#include "QDecNumber.hh"
#include "CurrencyData.h"
#include "SettingsGlobal.h"

class Money_t
{
public:
    Money_t();
    Money_t(double val);
    Money_t(const QString &val);
    CurrencyData::Currencies getCurrency() const {return currency_; }
    void setCurrency(CurrencyData::Currencies currency) {currency_ = currency; }

    QString verballyPL() const;

private:
    CurrencyData::Currencies currency_;
    QDecNumber value_;
    QDecContext context_;

private:
    short digit_(const QDecNumber &num, const int index) const;
    QString verballyPL1_999(const QDecNumber &val) const;
};

#endif // MONEY_T_H
