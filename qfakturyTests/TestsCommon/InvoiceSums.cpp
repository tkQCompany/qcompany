#include "InvoiceSums.h"

Money_t InvoiceSums::computeDiscountVal(const CommodityData &cd, const Money_t &netVal, const DecVal &discountPerCommodity, const DecVal &discountPerInvoice)
{
    Money_t ret;
    const DecVal one(1), hundred(100);
    const Money_t grossVal(computeGrossVal(cd, netVal));

    ret = grossVal * (discountPerCommodity / hundred); //value of discount per commodity
    ret = (grossVal - ret) * (one - discountPerInvoice / hundred); //value of gross after discounting per commodity
    ret = (grossVal - ret); //value of total discount

    return ret;
}


Money_t InvoiceSums::computeGrossVal(const CommodityData &cd, const Money_t &netVal)
{
    const DecVal one(1), hundred(100);
    const DecVal vatRate(cd.vat() / hundred);
    const Money_t grossVal(netVal * (one + vatRate));

    return grossVal;
}



Money_t InvoiceSums::computeNetVal(const CommodityData &cd, const int netValIndex)
{
    Money_t ret;
    const DecVal quantity(cd.quantity());

    switch(netValIndex)
    {
    case 0:
        ret = cd.net1() * quantity;
        break;
    case 1:
        ret = cd.net2() * quantity;
        break;
    case 2:
        ret = cd.net3() * quantity;
        break;
    case 3:
        ret = cd.net4() * quantity;
        break;
    default:
        qDebug("InvoiceSums::computeNetVal(): unexpected switch case value: netValIndex = %d", netValIndex);
        break;
    }

    return ret;
}
