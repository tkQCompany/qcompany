#include <QObject>
#include <QFile>
#include <QTextStream>

#include "InvoiceComposer.h"
#include "InvoiceTypeData.h"
#include "SettingsGlobal.h"

InvoiceComposer::InvoiceComposer(): filenameHtmlTemplate_(":/res/templates/template.html"), filenameCssStyle_("style.css"), isInvoiceDataLoaded_(false)
{
    invoiceHtmlTemplate_ = InvoiceComposer::readTextFileContent(filenameHtmlTemplate_);
    styleCssContent_ = InvoiceComposer::readTextFileContent(filenameCssStyle_);

}


void InvoiceComposer::setInvoiceData(const InvoiceData &invData)
{
    invoiceData_ = invData;
    isInvoiceDataLoaded_ = true;
}


QString InvoiceComposer::getInvoiceHtml() const
{   
    if(!isInvoiceDataLoaded_)
    {
        return QString("");
    }

    const int precisionMoney = 2;
    SettingsGlobal s;

    const QString logo(s.value(s.LOGO).toString());
    const QString stampStr(logo.isEmpty() ? QObject::trUtf8("Pieczęć wystawcy") : QString("<img src=\"%1\">").arg(logo));

    return invoiceHtmlTemplate_.arg(s.value(s.LANG).toString())
            .arg(InvoiceTypeData::name(invoiceData_.type()))
            .arg(styleCssContent_)
            .arg(stampStr)
            .arg(InvoiceTypeData::name(invoiceData_.type()))
            .arg(QObject::trUtf8("Nr: %1").arg(invoiceData_.invNumber()))
            .arg(QObject::trUtf8("Data wystawienia: %1").arg(invoiceData_.issuanceDate().toString(s.dateFormatExternal())))
            .arg(QObject::trUtf8("Data sprzedaży: %1").arg(invoiceData_.sellingDate().toString(s.dateFormatExternal())))
            .arg(QObject::trUtf8("ORYGINAŁ"))
            .arg(QString("<h1>Sprzedawca:</h1><ul>%1</ul>").arg(composeSellerIntoHtml()))
            .arg(QObject::trUtf8("<h1>Nabywca:</h1><ul><li>%1</li></ul>").arg(customer_.name()))
            .arg(composeProductsIntoHtml())
            .arg(QObject::trUtf8("Wartość Netto"))
            .arg(QObject::trUtf8("Kwota VAT"))
            .arg(QObject::trUtf8("Wartość Brutto"))
            .arg(QObject::trUtf8("Razem:"))
            .arg(netVal_.toString(precisionMoney))
            .arg((grossVal_ - netVal_).toString(precisionMoney))
            .arg(grossVal_.toString(precisionMoney))
            .arg(composeSummaryIntoHtml());
}


QString InvoiceComposer::composeProductsIntoHtml() const
{
    QString productsHTML;
    SettingsGlobal s;

    foreach(CommodityVisualData cvd, products_)
    {
        productsHTML += "<tr>";
        if(s.contains(s.keyName(s.ORDER_NUMBER)))
        {
            productsHTML += QString("<td>%1</td>").arg(cvd.ID());
        }

        if(s.contains(s.keyName(s.NAME)))
        {
            productsHTML += QString("<td>%1</td>").arg(cvd.name());
        }

        if(s.contains(s.keyName(s.PKWIU)))
        {
            productsHTML += QString("<td>%1</td>").arg(cvd.pkwiu());
        }

        if(s.contains(s.keyName(s.QUANTITY)))
        {
            productsHTML += QString("<td>%1</td>").arg(cvd.quantity().toString());
        }

        if(s.contains(s.keyName(s.INTERNAT_UNIT)))
        {
            productsHTML += QString("<td>%1</td>").arg(cvd.unit());
        }

        if(s.contains(s.keyName(s.NET_VAL)))
        {
            const int precision = 2;
            productsHTML += QString("<td>%1</td>").arg(cvd.net().toString(precision));
        }

        if(s.contains(s.keyName(s.DISCOUNT)))
        {
            productsHTML += QString("<td>%1</td>").arg(cvd.discount().toString());
        }

        if(s.contains(s.keyName(s.VAT_VAL)))
        {
            productsHTML += QString("<td>%1</td>").arg(cvd.vat().toString());
        }

        productsHTML += "</tr>";
    }

    return productsHTML;
}


QString InvoiceComposer::composeSellerIntoHtml() const
{
    QString sellerAttrList;
    SettingsGlobal s;

    s.beginGroup(s.categoryName(s.PRINT_FIELDS));
    if(s.value(s.DISPLAY_SELLER_NAME).toBool())
    {
        sellerAttrList += QString("<li>Nazwa: %1</li>").arg(seller_.name());
    }

    if(s.value(s.DISPLAY_SELLER_ADDRESS).toBool())
    {
        sellerAttrList += QString("<li>Ulica: %1</li>").arg(seller_.street());
    }

    if(s.value(s.DISPLAY_SELLER_LOCATION).toBool())
    {
        sellerAttrList += QString("<li>Miejscowość: %1</li>").arg(seller_.location());
    }

    if(s.value(s.DISPLAY_SELLER_TAXID).toBool())
    {
        sellerAttrList += QObject::trUtf8("<li>NIP: %1</li>").arg(seller_.tax_ident());
    }

    if(s.value(s.DISPLAY_SELLER_ACCOUNT).toBool())
    {
        sellerAttrList += QObject::trUtf8("<li>Nr konta: %1</li>").arg(seller_.account_name());
    }
    s.endGroup();

    const QString sellerHTML(QString("<h1>Sprzedawca:</h1><ul>%1</ul>").arg(sellerAttrList));
    return sellerHTML;
}



QString InvoiceComposer::composeSummaryIntoHtml() const
{
    const int precisionMoney = 2;
    QString summaryHTML(QString("<span>%1 %2 %3</span>")
                        .arg(QObject::trUtf8("Do zapłaty:"))
                        .arg(grossVal_.toString(precisionMoney)
                             .arg(CurrencyData::name(invoiceData_.currency()))));
    summaryHTML += QString("%1 %2")
            .arg(QObject::trUtf8("słownie:"))
            .arg(grossVal_.verballyPL());

    if(invoiceData_.paymentType() == PaymentTypeData::CASH)
    {
        summaryHTML += QObject::trUtf8("forma płatności: ") + PaymentTypeData::name(invoiceData_.paymentType()) + "<b>";
        summaryHTML += QObject::trUtf8("Zapłacono gotówką");
    }
//    else if((ui->comboBoxPayment->currentIndex() == ui->comboBoxPayment->count() -1) && (custPaymDataPtr != 0))
//    {
//        summaryHTML += "<span style=\"toPay\">";
//        summaryHTML += QString(QObject::trUtf8("Zapłacono: ") + custPaymDataPtr->payment1 + ": "
//                              +  s.numberToString(custPaymDataPtr->amount1) + " " + ui->comboBoxCurrency->currentText() + " "
//                              + custPaymDataPtr->date1.toString(s.dateFormatExternal()) + "<br>");
//        summaryHTML += QString(QObject::trUtf8("Zaległości: ") + custPaymDataPtr->payment2 + ": "
//                              +  s.numberToString(custPaymDataPtr->amount2) + " " + ui->comboBoxCurrency->currentText() + " "
//                              + custPaymDataPtr->date2.toString(s.dateFormatExternal()));
//        summaryHTML += "</span>";
//    }
//    else
//    {
//        summaryHTML += QObject::trUtf8("forma płatności: ") + ui->comboBoxPayment->currentText() + "<b>";
//        summaryHTML += "<span style=\"payDate\">";
//        summaryHTML += QObject::trUtf8("termin płatności: ") + ui->dateEditDayOfPayment->date().toString(s.dateFormatExternal());
//        summaryHTML += "</span></b>";
//    }

    summaryHTML += "<span class=\"additionalText\">"	+ invoiceData_.additText() + "</span>";
    summaryHTML += "</td>";
    summaryHTML += "<td width=\"3%\">&nbsp;</td>";
    summaryHTML += "<td width=\"48%\" valign=\"top\">";

    summaryHTML += "<table width=\"90%\" border=\"0\">";
    summaryHTML += "<tr class=\"stawkiHeader\"><td colspan=\"4\">";
    summaryHTML += QObject::trUtf8("Ogółem stawkami:");
    summaryHTML += "</td></tr>";
    //summaryHTML += getGroupedSums();
    summaryHTML += "<tr>";
    summaryHTML += "<td>&nbsp;</td>"; // netto
    summaryHTML += "<td>&nbsp;</td>"; // stawka
    summaryHTML += "<td>&nbsp;</td>"; // podatek
    summaryHTML += "<td>&nbsp;</td>"; // brutto
    summaryHTML += "</tr>";
    summaryHTML += "</table>";
    summaryHTML += "</td>";
    summaryHTML += "</tr>";
    summaryHTML += "</table>";
    summaryHTML += "</td></tr>";

    return summaryHTML;
}


QString InvoiceComposer::readTextFileContent(const QString &filename)
{
    QFile file(filename);
    QTextStream stream;
    QString ret;
    if(file.open(QIODevice::ReadOnly))
    {
        stream.setDevice(&file);
        ret = stream.readAll();
        file.close();
    }
    else
    {
        qDebug("File %s couldn't be opened.", qPrintable(filename));
    }

    return ret;
}



void InvoiceComposer::setGrossVal(const Money_t &grossVal)
{
    grossVal_ = grossVal;
}



void InvoiceComposer::setQuantity(const DecVal &quantity)
{
    quantity_ = quantity;
}



void InvoiceComposer::setNetVal(const Money_t &netVal)
{
    netVal_ = netVal;
}



void InvoiceComposer::setCustomer(const CounterpartyData &customer)
{
    customer_ = customer;
}



void InvoiceComposer::setSeller(const CounterpartyData &seller)
{
    seller_ = seller;
}



void InvoiceComposer::setProducts(const QList<CommodityVisualData> &products)
{
    products_ = products;
}

