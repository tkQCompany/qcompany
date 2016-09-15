#include <QObject>
#include <QFile>
#include <QTextStream>

#include "InvoiceComposer.h"
#include "InvoiceTypeData.h"
#include "SettingsGlobal.h"

InvoiceComposer::InvoiceComposer(): filenameHtmlTemplate_(":/res/templates/template.html"),
    filenameCssStyle_(":/res/templates/style.css"),
    isInvoiceDataLoaded_(false)
{
    invoiceHtmlTemplate_ = InvoiceComposer::readTextFileContent(filenameHtmlTemplate_);
    styleCssContent_ = InvoiceComposer::readTextFileContent(filenameCssStyle_);

}


void InvoiceComposer::setData(const InvoiceData &invData, const Money_t &netVal,
                                     const Money_t &grossVal, const DecVal &quantity,
                                     const CounterpartyData &buyer, const CounterpartyData &seller,
                                     const QList<CommodityVisualData> &products)
{
    invoiceData_ = invData;
    isInvoiceDataLoaded_ = true;
    netVal_ = netVal;
    grossVal_ = grossVal;
    quantity_ = quantity;
    buyer_ = buyer;
    seller_ = seller;
    products_ = products;
}


QString InvoiceComposer::getInvoiceHtml() const
{   
    if(!isInvoiceDataLoaded_)
    {
        return QString("");
    }

    SettingsGlobal s;

    const QString logo(s.value(s.LOGO_PATH).toString());
    const QString stampStr(logo.isEmpty() ? QObject::trUtf8("Pieczęć wystawcy") : QString("<img src=\"%1\"/>").arg(logo).toHtmlEscaped());

    Mustache::Renderer renderer;
    QVariantHash map;

    map["lang"] = s.value(s.LANG).toString();
    map["invoice_type"] = InvoiceTypeData::name(invoiceData_.type());
    map["css_style"] = styleCssContent_;
    map["logo"] = stampStr;

    map["invoice_header"] = true;
    map["invoice_number_flag"] = s.value(s.DISPLAY_INVOICE_NUMBER).toBool();
    map["invoice_number"] = QObject::trUtf8("Nr: %1").arg(invoiceData_.invNumber());
    map["invoice_issuance_date_flag"] = s.value(s.DISPLAY_ISSUANCE_DATE).toBool();
    map["invoice_issuance_date"] = QObject::trUtf8("Data wystawienia: %1").arg(invoiceData_.issuanceDate().toString(s.dateFormatExternal()));
    map["invoice_selling_date_flag"] = s.value(s.DISPLAY_SELLING_DATE).toBool();
    map["invoice_selling_date"] = QObject::trUtf8("Data sprzedaży: %1").arg(invoiceData_.sellingDate().toString(s.dateFormatExternal()));
    map["invoice_original_copy_flag"] = s.value(s.DISPLAY_ORIGINAL_COPY).toBool();
    map["invoice_original_copy"] = invoiceData_.isOriginal() ? QObject::trUtf8("ORYGINAŁ") : QObject::trUtf8("KOPIA");

    map["seller"] = true;
    map["seller_name_flag"] = s.value(s.DISPLAY_SELLER_NAME).toBool();
    map["seller_name"] = seller_.name();
    map["seller_street_flag"] = s.value(s.DISPLAY_SELLER_STREET).toBool();
    map["seller_street"] = seller_.street();
    map["seller_location_flag"] = s.value(s.DISPLAY_SELLER_LOCATION).toBool();
    map["seller_location"] = seller_.location();
    map["seller_taxid_flag"] = s.value(s.DISPLAY_SELLER_TAXID).toBool();
    map["seller_taxid"] = seller_.tax_ident();
    map["seller_account_flag"] = s.value(s.DISPLAY_SELLER_ACCOUNT).toBool();
    map["seller_account"] = seller_.account_name();

    map["buyer"] = true;
    map["buyer_name_flag"] = s.value(s.DISPLAY_BUYER_NAME).toBool();
    map["buyer_name"] = buyer_.name();
    map["buyer_street_flag"] = s.value(s.DISPLAY_BUYER_STREET).toBool();
    map["buyer_street"] = buyer_.street();
    map["buyer_location_flag"] = s.value(s.DISPLAY_BUYER_LOCATION).toBool();
    map["buyer_location"] = buyer_.location();
    map["buyer_taxid_flag"] = s.value(s.DISPLAY_BUYER_TAXID).toBool();
    map["buyer_taxid"] = buyer_.tax_ident();
    map["buyer_account_flag"] = s.value(s.DISPLAY_BUYER_ACCOUNT).toBool();
    map["buyer_account"] = buyer_.account_name();

    const QVariantList products = composeProducts();
    map["products"] = products;

    Mustache::QtVariantContext invoiceContext(map);
    return renderer.render(invoiceHtmlTemplate_, &invoiceContext);


//    return invoiceHtmlTemplate_.arg(s.value(s.LANG).toString())
//            .arg(InvoiceTypeData::name(invoiceData_.type()))
//            .arg(styleCssContent_)
//            .arg(stampStr)
//            .arg(InvoiceTypeData::name(invoiceData_.type()))
//            .arg(QObject::trUtf8("Nr: %1").arg(invoiceData_.invNumber()))
//            .arg(QObject::trUtf8("Data wystawienia: %1").arg(invoiceData_.issuanceDate().toString(s.dateFormatExternal())))
//            .arg(QObject::trUtf8("Data sprzedaży: %1").arg(invoiceData_.sellingDate().toString(s.dateFormatExternal())))
//            .arg(QObject::trUtf8("ORYGINAŁ"))
//            .arg(QString("<h1>Sprzedawca:</h1><ul>%1</ul>").arg(composeSellerIntoHtml()))
//            .arg(QObject::trUtf8("<h1>Nabywca:</h1><ul><li>%1</li></ul>").arg(buyer_.name()))
//            .arg(composeProducts())
//            .arg(QObject::trUtf8("Wartość Netto"))
//            .arg(QObject::trUtf8("Kwota VAT"))
//            .arg(QObject::trUtf8("Wartość Brutto"))
//            .arg(QObject::trUtf8("Razem:"))
//            .arg(netVal_.toString(precisionMoney))
//            .arg((grossVal_ - netVal_).toString(precisionMoney))
//            .arg(grossVal_.toString(precisionMoney))
//            .arg(composeSummaryIntoHtml());
}


QVariantList InvoiceComposer::composeProducts() const
{
    QVariantList products;
    SettingsGlobal s;

    foreach(CommodityVisualData cvd, products_)
    {
        QVariantHash product;
        if(s.contains(s.ORDER_NUMBER))
        {
            product["prod_id"] = cvd.ID();
        }

        if(s.contains(s.NAME))
        {
            product["prod_name"] = cvd.name();
        }

        if(s.contains(s.PKWIU))
        {
            product["prod_pkwiu"] = cvd.pkwiu();
        }

        if(s.contains(s.QUANTITY))
        {
            product["prod_quantity"] = cvd.quantity().toString();
        }

        if(s.contains(s.INTERNAT_UNIT))
        {
            product["prod_unit"] = cvd.unit();
        }

        if(s.contains(s.NET_VAL))
        {
            const int precision = 2;
            product["prod_net_value"] = cvd.net().toString(precision);
        }

        if(s.contains(s.DISCOUNT))
        {
            product["prod_discount"] = cvd.discount().toString();
        }

        if(s.contains(s.VAT_VAL))
        {
            product["prod_vat_value"] = cvd.vat().toString();
        }

        products << product;
    }

    return products;
}


QString InvoiceComposer::composeSellerIntoHtml() const
{
    QString sellerAttrList;
    SettingsGlobal s;

    if(s.value(s.DISPLAY_SELLER_NAME).toBool())
    {
        sellerAttrList += QString("<li>Nazwa: %1</li>").arg(seller_.name());
    }

    if(s.value(s.DISPLAY_SELLER_STREET).toBool())
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
