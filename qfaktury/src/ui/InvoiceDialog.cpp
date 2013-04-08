#include <QPrintPreviewDialog>
#include <QPrinter>

#include "InvoiceDialog.h"
#include "ui_InvoiceDialog.h"
#include "Database.h"


InvoiceDialog::InvoiceDialog(QWidget *parent, Database *db, InvoiceTypeData::Type invoiceType, const QModelIndex &idEdit, const bool newPImpl) :
    QDialog(parent)
{
    if(newPImpl)
    {
        pImpl_ = new InvoiceDialogImpl(parent, db);
    }
    pImpl_->ui->setupUi(this);
    pImpl_->init(invoiceType, idEdit);
}


/**
 * @brief
 *
 */
InvoiceDialog::~InvoiceDialog()
{
    delete pImpl_;
}



/**
 * @brief
 *
 */
//void InvoiceDialog::makeInvoiceHeaderHTML(const int invoiceType)
//{
//    invStrList += "<html><head>";
//    invStrList += "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />";
//    invStrList += "<meta name=\"creator\" value=\"http://www.e-linux.pl\" />";
//    invStrList += "</head>";

//    invStrList += "<title>"+ InvoiceTypeData::InvoiceTypeToString(invoiceType)  +"</title>";

//    invStrList += "<style type=\"text/css\"> ";
//    SettingsGlobal s;
//    QFile file(s.getTemplate());
//    if (file.open(QIODevice::ReadOnly))
//    {
//        QTextStream stream(&file);
//        QString line;
//        while (!stream.atEnd())
//        {
//            line = stream.readLine();
//            invStrList += line;
//        }
//        file.close();
//    }
//    else
//    {
//        qWarning() << "Could not open CSS file: " << file.fileName();
//    }
//    invStrList += "</style>";
//    invStrList += "<body>";
//}


/**
 * @brief
 *
 * @param sellDate
 * @param breakPage
 * @param original
 */
//void InvoiceDialog::makeInvoiceHeader(const int invoiceType, const bool sellDate, const bool breakPage, const bool original)
//{
//    QString breakPageStr = "class=\"page_break\"";
//    if (breakPage == false) breakPageStr = "";

//    invStrList += "<table comment=\"header table\" width=\"100%\" border=\"0\"" + breakPageStr + ">";
//    invStrList += "<tr>";
//    invStrList += "<td width=\"60%\" align=\"center\" valign=\"bottom\">";
//    invStrList += "<span class=\"stamp\">";

//    SettingsGlobal s;
//    const QString logo(s.value(s.LOGO)).toString());
//    if (!logo.isEmpty())
//    {
//        invStrList += "<img src=\"" + logo + "\" width=\"100\" " + " height=\"100\"+ >";
//    }
//    else
//    {
//        invStrList += trUtf8("Pieczęć wystawcy");
//    }
//    invStrList += "</span>";
//    invStrList += "</td>";

//    invStrList += "<td align=\"right\">";
//    invStrList += "<span style=\"font-size:12pt; font-weight:600\">";
//    invStrList += InvoiceTypeData::InvoiceTypeToString(invoiceType) + "<br/>";
//    invStrList += trUtf8("Nr: ") + lineEditInvNumber->text() + "<br></span>";
//    invStrList += "<span class=\"dates\">" + trUtf8("Data wystawienia: ")
//            + dateEditDateOfIssuance->date().toString(s.getDateFormat()) + "<br>";

//    if (sellDate)
//        invStrList += trUtf8("Data sprzedaży: ")
//                + dateEditDateOfSell->date().toString(s.getDateFormat())
//                + "<br>";
//    invStrList += "</span></td><td width=\"3%\">&nbsp;</td>";
//    invStrList += "</tr>";
//    invStrList += "<tr>";
//    invStrList += "<td colspan=\"2\" align=\"right\" valign=\"top\"><br>";
//    if (original) {
//        invStrList += trUtf8("ORYGINAŁ");
//    } else {
//        invStrList += trUtf8("KOPIA");
//    }
//    invStrList += "<br></td><td width=\"3%\">&nbsp;</td>";
//    invStrList += "</tr>";
//    invStrList += "</table>";
//    invStrList += "<hr>";
//    invStrList += "</td></tr>";
//}


/**
 * @brief
 *
 */
//void InvoiceDialog::makeInvoiceBody()
//{
//    invStrList += "<tr><td>";

//    invStrList += "<table width=\"100%\" border=\"0\">";
//    invStrList += "<tr class=\"persons\">";
//    invStrList += "<td width=\"20\">&nbsp;</td>";
//    invStrList += "<td width=\"48%\"> ";
//    invStrList += trUtf8("Sprzedawca:")+"<br>";

//    //QSettings userSettings("elinux", "user");
//    SettingsGlobal s;

//    s.beginGroup("printpos");
//    if (s.value(s.USER_NAME)).toBool())
//        invStrList += s.value(s.USER_NAME)).toString() + "<br>";
//    if (s.value(s.USER_ADDRESS)).toBool())
//        invStrList += s.value(s.USER_ADDRESS)).toString() + "<br>"; // trUtf8("Ul. ") +
////    if (settings.value("usermiejscowosc").toBool())
////        invStrList += userSettings.value("zip").toString() + " ";
//    if (s.value(s.USER_LOCATION)).toBool())
//        invStrList += s.value(s.USER_LOCATION)).toString() + "<br>";
//    if (s.value(s.USER_TAXID)).toBool())
//        invStrList += trUtf8("NIP: ") + s.value(s.USER_TAXID)).toString() + "<br>";
//    if (s.value(s.USER_ACCOUNT)).toBool())
//        invStrList += trUtf8("Nr konta: ")
//                + s.value(s.USER_ACCOUNT)).toString().replace("-", " ") + "<br>";
//    s.endGroup();

//    invStrList += "</td>";
//    invStrList += "<td width=\"20\">&nbsp;</td>";
//    invStrList += "<td width=\"48%\">";
//    invStrList += trUtf8("Nabywca:") + "<br>" + comboBoxCounterparties->currentText().replace(",", "<br>") + "<br>";
//    invStrList += "</td>";
//    invStrList += "</tr>";
//    invStrList += "</table>";

//    invStrList += "<hr>";
//    invStrList += "</td></tr>";
//}

/**
 * @brief
 *
 */
//void InvoiceDialog::makeInvoiceProductsHeader()
//{
//    SettingsGlobal s;
//    int currentPercent = 0;
//    invStrList += "<tr align=\"center\" valign=\"middle\" class=\"productsHeader\" >";

//    if (s.value(s.ORDER_NUMBER)).toBool())
//    {
//        currentPercent = 3;
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(currentPercent) + "%\">" + trUtf8("Lp.") + "</td>";
//        currentPercent = 0;
//    } else {
//        currentPercent += 3;
//    }
//    if (s.value(s.NAME)).toBool())
//    {
//        currentPercent += 25;
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(currentPercent) + "%\">" + trUtf8("Nazwa") + "</td>";
//        currentPercent = 0;
//    } else {
//        currentPercent += 25;
//    }
//    if (s.value(s.CODE)).toBool())
//    {
//        currentPercent += 7;
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(currentPercent) + "%\">" + trUtf8("Kod") + "</td>";
//        currentPercent = 0;
//    } else {
//        currentPercent += 7;
//    }
//    if (s.value(s.PKWIU)).toBool())
//    {
//        currentPercent += 7;
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(currentPercent) + "%\">" + trUtf8("PKWiU") + "</td>";
//        currentPercent = 0;
//    } else {
//        currentPercent += 7;
//    }
//    if (s.value(s.QUANTITY)).toBool())
//    {
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(7) + "%\">" + trUtf8("Ilość") + "</td>";
//    } else {
//        currentPercent += 7;
//    }
//    if (s.value(s.INTERNAT_UNIT)).toBool())
//    {
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(3) + "%\">" + trUtf8("jm.") + "</td>";
//    } else {
//        currentPercent += 3;
//    }
//    if (s.value(s.UNIT_PRICE)).toBool())
//    {
//        currentPercent += 7;
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(currentPercent) + "%\">" + trUtf8("Cena jdn.") + "</td>";
//        currentPercent = 0;
//    } else {
//        currentPercent += 7;
//    }
//    if (s.value(s.NET_VAL)).toBool())
//    {
//        currentPercent += 7;
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(currentPercent) + "%\">" + trUtf8("Wartość Netto") + "</td>";
//        currentPercent = 0;
//    } else {
//        currentPercent += 7;
//    }
//    if (s.value(s.DISCOUNT)).toBool())
//    {
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(3) + "%\">" + trUtf8("Rabat %") + "</td>";
//    } else {
//        currentPercent += 3;
//    }
//    if (s.value(s.DISCOUNT_VAL)).toBool())
//    {
//        currentPercent += 3;
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(currentPercent) + "%\">" + trUtf8("Rabat Wartość") + "</td>";
//        currentPercent = 0;
//    } else {
//        currentPercent += 3;
//    }
//    if (s.value(s.NET_AFTER)).toBool())
//    {
//        currentPercent += 7;
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(currentPercent) + "%\">" + trUtf8("Netto po rabacie") + "</td>";
//        currentPercent = 0;
//    } else {
//        currentPercent += 7;
//    }
//    if (s.value(s.VAT_VAL)).toBool())
//    {
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(7) + "%\">" + trUtf8("Stawka VAT") + "</td>";
//    } else {
//        currentPercent += 7;
//    }
//    if (s.value(s.VAT_PRICE)).toBool())
//    {
//        currentPercent += 7;
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(currentPercent) + "%\">" + trUtf8("Kwota Vat") + "</td>";
//        currentPercent = 0;
//    } else {
//        currentPercent += 7;
//    }
//    if (s.value(s.GROSS_VAL)).toBool())
//    {
//        currentPercent += 7;
//        invStrList += "<td align=\"center\" width=\""+ s.numberToString(currentPercent) + "%\">" + trUtf8("Wartość Brutto") + "</td>";
//        currentPercent = 0;
//    } else {
//        currentPercent += 7;
//    }
//    invStrList += "</tr>";
//}

/**
 * @brief
 *
 */
//void InvoiceDialog::makeInvoiceProducts()
//{
//    SettingsGlobal s;

//    invStrList += "<tr><td>";
//    invStrList += "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\" width=\"100%\">";

//    makeInvoiceProductsHeader();

//    for (int i = 0; i < tableWidgetCommodities->rowCount(); ++i)
//    {
//        invStrList += "<tr valign=\"middle\" align=\"center\" class=\"products\">";
//        // lp, nazwa, kod, pkwiu, ilosc, jm, rabat, cena jm., netto, vat, brutto
//        if (s.value(s.ORDER_NUMBER)).toBool())
//            invStrList += "<td align=\"center\" >" + s.numberToString(i + 1) + "</td>";
//        if (s.value(s.NAME)).toBool())
//            invStrList += "<td align=\"left\">" + tableWidgetCommodities->item(i, CommodityVisualFields::NAME)->text() + "</td>";
//        //if (s.value(s.CODE)).toBool())
//          //  invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, CommodityVisualFields::)->text() + "</td>";
//        if (s.value(s.PKWIU)).toBool())
//            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, CommodityVisualFields::PKWIU)->text() + "</td>";
//        if (s.value(s.QUANTITY)).toBool())
//            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, CommodityVisualFields::QUANTITY)->text() + "</td>";
//        if (s.value(s.INTERNAT_UNIT)).toBool())
//            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, CommodityVisualFields::UNIT)->text() + "</td>";
//        //if (s.value(s.UNIT_PRICE)).toBool())
//          //  invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, CommodityVisualFields::)->text() + "</td>";
//        const double discountVal = s.stringToDouble(tableWidgetCommodities->item(i, CommodityVisualFields::NET)->text()) *
//                (tableWidgetCommodities->item(i, 6)->text().toDouble() * 0.01);
//        const double nettMinusDisc = s.stringToDouble(tableWidgetCommodities->item(i, CommodityVisualFields::NET)->text()) - discountVal;
//        if (s.value(s.NET_VAL)).toBool())
//            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, CommodityVisualFields::NET)->text()	+ "</td>"; // netto
//        if (s.value(s.DISCOUNT)).toBool())
//            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, CommodityVisualFields::DISCOUNT)->text()	+ "%</td>"; // rabat
//        if (s.value(s.DISCOUNT_VAL)).toBool())
//            invStrList += "<td align=\"center\" >" + s.numberToString(discountVal, 'f',  2)	+ "</td>";
//        if (s.value(s.NET_AFTER)).toBool())
//            invStrList += "<td align=\"center\" >" + s.numberToString(nettMinusDisc, 'f', 2) + "</td>";
//        if (s.value(s.VAT_VAL)).toBool())
//            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, CommodityVisualFields::VAT)->text()	+ "%</td>";
//        const double vatPrice = s.stringToDouble(tableWidgetCommodities->item(i, 10)->text())
//                - s.stringToDouble(tableWidgetCommodities->item(i, CommodityVisualFields::VAT)->text()); // brutt-nett :)
//        if (s.value(s.VAT_PRICE)).toBool())
//            invStrList += "<td align=\"center\" >" + s.numberToString(vatPrice, 'f', 2) + "</td>";
//        if (s.value(s.GROSS_VAL)).toBool())
//            invStrList += "<td align=\"center\" >" + tableWidgetCommodities->item(i, 10)->text() + "</td>";
//        invStrList += "</tr>";
//    }

//    invStrList += "</table>";
//}

/**
 * @brief
 *
 */
//void InvoiceDialog::makeInvoiceSumm()
//{
//    SettingsGlobal s;

//    const double vatPrice = s.stringToDouble(labelSumGrossVal->text()) - s.stringToDouble(labelSumNetVal->text());
//    invStrList += "<br><table width=\"100%\" border=\"0\" cellpadding=\"5\">";
//    invStrList += "<tr class=\"productsSumHeader\" valign=\"middle\">";
//    invStrList += "<td width=\"67%\" align=\"center\">&nbsp;</td>";
//    invStrList += "<td width=\"11%\" align=\"center\">" + trUtf8("Wartość Netto") + "</td>"; // netto
//    invStrList += "<td width=\"11%\" align=\"center\">" + trUtf8("Kwota VAT") + "</td>";// vat
//    invStrList += "<td width=\"11%\" align=\"center\">" + trUtf8("Wartość Brutto") + "</td>"; // brutto
//    invStrList += "</tr><tr class=\"productsSum\">";
//    invStrList += "<td align=\"right\">" + trUtf8("Razem:") + "</td>";
//    invStrList += "<td align=\"center\">" + labelSumNetVal->text() + "</td>"; // netto
//    invStrList += "<td align=\"center\">" + s.numberToString(vatPrice, 'f', 2) + "</td>";// vat
//    invStrList += "<td align=\"center\">" + labelSumGrossVal->text() + "</td>"; // brutto
//    invStrList += "</tr>";
//    invStrList += "</table><br><br>";
//}

/**
 * @brief
 *
 */
//void InvoiceDialog::makeInvoiceSummAll()
//{
//    SettingsGlobal s;

//    invStrList += "</td></tr>"; // closing products row
//    invStrList += "<tr comment=\"podsumowanie\"><td>";

//    invStrList += "<table width=\"100%\" border=\"0\">";
//    invStrList += "<tr class=\"summary\">";
//    invStrList += "<td width=\"3%\">&nbsp;</td>";
//    invStrList += "<td width=\"48%\"><span style=\"toPay\">";
//    invStrList += trUtf8("Do zapłaty: ") + labelSumGrossVal->text() + " "
//            + comboBoxCurrency->currentText() + "</span><br>";

//    invStrList += trUtf8("słownie:")
//            + ConvertAmount::convertPL(labelSumGrossVal->text(), comboBoxCurrency->currentText()) + "<br>";
//    //QString paym1 = sett().value("paym1").toString();
//    if (comboBoxPayment->currentIndex() == 0) {
//        invStrList += trUtf8("forma płatności: ") + comboBoxPayment->currentText() + "<br><b>";
//        invStrList += trUtf8("Zapłacono gotówką") + "<br>";
//    } else if ((comboBoxPayment->currentIndex() == comboBoxPayment->count() -1) && (custPaymData != NULL)) {
//        invStrList += "<span style=\"toPay\">";
//        invStrList += QString(trUtf8("Zapłacono: ") + custPaymData->payment1 + ": "
//                              +  s.numberToString(custPaymData->amount1) + " " + comboBoxCurrency->currentText() + " "
//                              + custPaymData->date1.toString(s.getDateFormat()) + "<br>");
//        invStrList += QString(trUtf8("Zaległości: ") + custPaymData->payment2 + ": "
//                              +  s.numberToString(custPaymData->amount2) + " " + comboBoxCurrency->currentText() + " "
//                              + custPaymData->date2.toString(s.getDateFormat()));
//        invStrList += "</span>";
//    }  else {
//        invStrList += trUtf8("forma płatności: ") + comboBoxPayment->currentText() + "<br><b>";
//        invStrList += "<span style=\"payDate\">";
//        invStrList += trUtf8("termin płatności: ")
//                + dateEditDayOfPayment->date().toString(s.getDateFormat())	+ "<br>";
//        invStrList += "</span>";
//    }

//    invStrList += "</b><br><br>";
//    invStrList += "<span class=\"additionalText\">"	+ lineEditAdditionalText->text() + "</span>";
//    invStrList += "</td>";
//    invStrList += "<td width=\"3%\">&nbsp;</td>";
//    invStrList += "<td width=\"48%\" valign=\"top\">";

//    invStrList += "<table width=\"90%\" border=\"0\">";
//    invStrList += "<tr class=\"stawkiHeader\"><td colspan=\"4\">";
//    invStrList += trUtf8("Ogółem stawkami:");
//    invStrList += "</td></tr>";
//    invStrList += getGroupedSums();
//    invStrList += "<tr>";
//    invStrList += "<td>&nbsp;</td>"; // netto
//    invStrList += "<td>&nbsp;</td>"; // stawka
//    invStrList += "<td>&nbsp;</td>"; // podatek
//    invStrList += "<td>&nbsp;</td>"; // brutto
//    invStrList += "</tr>";
//    invStrList += "</table>";

//    invStrList += "</td>";
//    invStrList += "</tr>";
//    invStrList += "</table>";

//    invStrList += "</td></tr>";
//}

/**
 * @brief
 *
 */
//void InvoiceDialog::makeInvoiceFooter() {
//    invStrList += "<tr comment=\"podpis\" align=\"center\"><td>";
//    invStrList += "<br><br><br><br>";

//    invStrList += "<table width=\"80%\" border=\"0\">";
//    invStrList += "<tr>";
//    invStrList += "<td width=\"3%\">&nbsp;</td>";
//    invStrList += "<td width=\"43%\" align=\"center\"> ";
//    invStrList += "<hr width=\"100%\" noshade=\"noshade\" color=\"black\" />";
//    invStrList += "</td>";
//    invStrList += "<td width=\"7%\">&nbsp;</td>";
//    invStrList += "<td width=\"3%\">&nbsp;</td>";
//    invStrList += "<td width=\"43%\" align=\"center\"> ";
//    invStrList += "<hr width=\"100%\" noshade=\"noshade\" color=\"black\" />";
//    invStrList += "</td>";
//    invStrList += "</tr>";
//    invStrList += "<tr class=\"signature\">";
//    invStrList += "<td width=\"3%\">&nbsp;</td>";
//    invStrList += "<td width=\"43%\" align=\"center\"> ";
//    invStrList += trUtf8("Imię i nazwisko osoby upoważnionej")
//            + "<br>" + trUtf8(" do wystawienia faktury VAT");
//    invStrList += "</td>";
//    invStrList += "<td width=\"7%\">&nbsp;</td>";
//    invStrList += "<td width=\"3%\">&nbsp;</td>";
//    invStrList += "<td width=\"43%\" align=\"center\">";
//    invStrList += trUtf8("Imię i nazwisko osoby upoważnionej")
//            + "<br>" + trUtf8(" do odbioru faktury VAT");
//    invStrList += "</td>";
//    invStrList += "</tr>";
//    invStrList += "</table>";

//    invStrList += "</td></tr>";
//    invStrList += "</table>";

//}

/**
 * @brief
 *
 */
//void InvoiceDialog::makeInvoiceFooterHtml() {
//    invStrList += "</body>";
//    invStrList += "</html>";
//}
