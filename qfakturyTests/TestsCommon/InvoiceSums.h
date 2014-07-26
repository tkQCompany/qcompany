#ifndef INVOICESUMS_H
#define INVOICESUMS_H

#include "Money_t.h"
#include "CommodityData.h"

class InvoiceSums
{
public:
    static Money_t computeDiscountVal(const CommodityData &cd, const Money_t &netVal, const DecVal &discountPerCommodity, const DecVal &discountPerInvoice);
    static Money_t computeGrossVal(const CommodityData &cd, const Money_t &netVal);
    static Money_t computeNetVal(const CommodityData &cd, const int netValIndex);
};

#endif // INVOICESUMS_H
