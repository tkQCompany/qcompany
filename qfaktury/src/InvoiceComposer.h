#ifndef INVOICECOMPOSER_H
#define INVOICECOMPOSER_H

#include <QString>

#include "mustache.h"

#include "InvoiceData.h"
#include "CommodityVisualData.h"
#include "CounterpartyData.h"


class InvoiceComposer
{
public:
    InvoiceComposer();

    void setData(const InvoiceData &invData, const Money_t &netVal, const Money_t &grossVal, const DecVal &quantity,
                        const CounterpartyData &buyer, const CounterpartyData &seller,
                        const QList<CommodityVisualData> &products);
    QString getInvoiceHtml() const;

private:
    QVariantList composeProducts() const;
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
    CounterpartyData buyer_;
    Money_t netVal_;
    DecVal quantity_;
    Money_t grossVal_;
};

#endif // INVOICECOMPOSER_H
