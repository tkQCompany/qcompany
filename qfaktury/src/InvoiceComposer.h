#ifndef INVOICECOMPOSER_H
#define INVOICECOMPOSER_H

#include <QString>

#include "InvoiceData.h"
#include "CommodityVisualData.h"
#include "CounterpartyData.h"


class InvoiceComposer
{
public:
    InvoiceComposer();

    void setInvoiceData(const InvoiceData &invData);
    QString getInvoiceHtml() const;
    void setProducts(const QList<CommodityVisualData> &products);
    void setSeller(const CounterpartyData &seller);
    void setCustomer(const CounterpartyData &customer);
    void setNetVal(const Money_t &netVal);
    void setQuantity(const DecVal &quantity);
    void setGrossVal(const Money_t &grossVal);

private:
    QString composeProductsIntoHtml() const;
    QString composeSellerIntoHtml() const;
    QString composeSummaryIntoHtml() const;
    static QString readTextFileContent(const QString &filename);

private:
    QString filenameHtmlTemplate_;
    QString filenameCssStyle_;
    QString filenameLogo_;
    QString invoiceHtmlTemplate_;
    QString styleCssContent_;
    InvoiceData invoiceData_;
    bool isInvoiceDataLoaded_;
    QList<CommodityVisualData> products_;
    CounterpartyData seller_;
    CounterpartyData customer_;
    Money_t netVal_;
    DecVal quantity_;
    Money_t grossVal_;
};

#endif // INVOICECOMPOSER_H
