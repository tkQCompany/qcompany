#include "CorrectiveInvoiceDialog.h"

// constructor
CorrectiveInvoiceDialog::CorrectiveInvoiceDialog(QWidget *parent, Database *db):
    InvoiceDialog(parent, db)
{
    init_();
}

CorrectiveInvoiceDialog::~CorrectiveInvoiceDialog()
{
}

/* Init
 */
void CorrectiveInvoiceDialog::init_ (/*const bool mode*/)
{
    labelReason1 = new QLabel(this);
    labelReason1->setText(trUtf8("Przyczyna korekty:"));
    labelReason1->setAlignment(Qt::AlignRight);
    //addDataLabels->addWidget(labelReason1);

    reasonCombo = new QComboBox(this);
    SettingsGlobal s;
    reasonCombo->addItems(s.value(s.keyName(s.CORRECTION_REASON)).toString().split("|"));
    //addData->addWidget(reasonCombo);

    labelSumNet->setText(trUtf8("Wartość korekty:"));
    labelDiscount2->setText(trUtf8("Wartość faktury:"));
    labelSumGross->setText(trUtf8("Do zapłaty:"));

    setWindowTitle(InvoiceTypeData::InvoiceTypeToString(InvoiceTypeData::CORRECTIVE_VAT));
    comboBoxInvoiceType->setCurrentIndex(InvoiceTypeData::CORRECTIVE_VAT - 1);
    origGrossTotal = -1;

    //editMode = mode;

    // connects
    connect(reasonCombo, SIGNAL(currentIndexChanged (QString)), this, SLOT(textChanged(QString)));
}

//*********************************************** SLOTS START ****************************************/



/** Slot
 *  Generate invoice and show preview
 */
void CorrectiveInvoiceDialog::printInvoice()
{
    invStrList.clear();

    makeInvoiceHeaderHTML(comboBoxInvoiceType->currentIndex() + 1);

    makeInvoiceHeader(comboBoxInvoiceType->currentIndex() + 1, false, false, true);
    makeInvoiceBody();
    makeInvoceProductsTitle(0);
    makeBeforeCorrProducts();
    makeBeforeCorrSumm();
    makeInvoceProductsTitle(1);
    makeInvoiceProducts();
    makeInvoiceSumm();
    makeInvoiceSummAll();
    makeInvoiceFooter();

    SettingsGlobal s;
    const int numberOfCopies = s.value(s.keyName(s.NUMBER_OF_COPIES), 2).toInt();
    for (int i = 1; i <= numberOfCopies; i++)
    {
        makeInvoiceHeader(comboBoxInvoiceType->currentIndex() + 1, false, true, false);
        makeInvoiceBody();
        makeInvoceProductsTitle(0);
        makeBeforeCorrProducts();
        makeBeforeCorrSumm();
        makeInvoceProductsTitle(1);
        makeInvoiceProducts();
        makeInvoiceSumm();
        makeInvoiceSummAll();
        makeInvoiceFooter();
    }

    makeInvoiceFooterHtml();

    print();
}

/** Slot
 *  Validate close and save if requested
 */
void CorrectiveInvoiceDialog::canQuit()
{
    if (!unsaved)
    {
        accept();
    }
    else
    {
        if (QMessageBox::warning(this, qApp->applicationName(), trUtf8("Dane zostały zmienione, czy chcesz zapisać?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            reject();
        }
        else
        {
            saveInvoice();
            if (saveFailed)
            {
                return;
            }
            accept();
        }
    }
}
//*********************************************** SLOTS END ****************************************/

/** Reads correction xml
 */
void CorrectiveInvoiceDialog::readCorrData(const QString &invFileName)
{
    lineEditInvNumber->setEnabled(false);

    setWindowTitle(trUtf8("Edytuje korektę"));

    QDomDocument doc("invoice");
    //fName = invFileName;

    SettingsGlobal s;
    QFile file(s.getWorkingDir() + s.getDataDir() + "/" + invFileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug("file doesn't exist"); //TODO
        return;
    }
    else
    {
        QTextStream stream(&file);
        if (!doc.setContent(stream.readAll()))
        {
            file.close();
            return;
        }
    }

    const QDomElement root(doc.documentElement());
    lineEditInvNumber->setText(root.attribute("no"));
    dateEditDateOfSell->setDate(QDate::fromString(root.attribute("sellingDate"), s.getDateFormat()));
    dateEditDateOfIssuance->setDate(QDate::fromString(root.attribute("issueDate"),	s.getDateFormat()));

    invData.invNumber = root.attribute("originalInvoiceNo");

    QDomNode tmp(root.firstChild());
    tmp = tmp.toElement().nextSibling(); // buyer
    const QDomElement buyer(tmp.toElement());
//    lineEditCounterpartyName->setText(buyer.attribute("name") + "," + buyer.attribute(
//                           "city") + "," + buyer.attribute("street") + "," + trUtf8("NIP: ")
//                       + buyer.attribute("tic"));
//    lineEditCounterpartyName->setCursorPosition(1);

    //*********************** Load Products Vars ***************************

    static const char *towarColumns[] = { "id", "name", "code", "PKWiU",
                                          "quantity", "quantityType", "discount", "price", "nett",
                                          "vatBucket", "gross" };

    //*********************** Load Products After ***************************
    tmp = tmp.toElement().nextSibling(); // product after
    QDomElement product(tmp.toElement());
    int towCount = product.attribute("productsCount").toInt();
    spinBoxDiscount->setValue(product.attribute("discount").toInt());

    towCount = product.attribute("productsCount").toInt();
    QDomElement towar = product.firstChild().toElement();

    //tableViewCommodities->setRowCount(towCount);
    for (int i = 0; i < towCount; ++i)
    {
        for (int j = 0; j < int(sizeof(towarColumns) / sizeof(*towarColumns)); j++)
        {
            //tableWidgetCommodities->setItem(i, j, new QTableWidgetItem(towar.attribute(towarColumns[j])));
        }
        towar = towar.nextSibling().toElement();
    }

    //*********************** Load Products Before ***************************
    tmp = tmp.toElement().nextSibling(); // product before
    product = tmp.toElement();
    towCount = product.attribute("productsCount").toInt();
    towar = product.firstChild().toElement();

    // those fields are not stored in correction xml... for now
    //invData.counterpartyName = lineEditCounterpartyName->text();
    invData.paymentDate = dateEditDayOfPayment->date();
    invData.sellingDate = dateEditDateOfSell->date();
    invData.issuanceDate = dateEditDateOfIssuance->date();
    invData.paymentID = comboBoxPayment->currentIndex();
    invData.currencyID = comboBoxCurrency->currentIndex();
    invData.additText = lineEditAdditionalText->text();

    for (int i = 0; i < towCount; ++i)
    {
        CommodityData commodity;
//        commodity.id = towar.attribute(towarColumns[0]).toInt();
//        commodity.name = towar.attribute(towarColumns[1]);
//        //commodity.postalCode = towar.attribute(towarColumns[2]);
//        commodity.pkwiu = towar.attribute(towarColumns[3]);
//        commodity.quantity = towar.attribute(towarColumns[4]).toDouble();
//        //commodity.unit_id = towar.attribute(towarColumns[5]);
//        commodity.discount = towar.attribute(towarColumns[6]).toDouble();
//        commodity.price = towar.attribute(towarColumns[7]).toDouble();
//        commodity.net1 = towar.attribute(towarColumns[8]).toDouble();
//        commodity.vat = towar.attribute(towarColumns[9]).toInt();
//        commodity.gross = towar.attribute(towarColumns[10]).toDouble();
        invData.products[i] = commodity;
        towar = towar.nextSibling().toElement();
    }

    tmp = tmp.toElement().nextSibling();
    QDomElement additional = tmp.toElement();
    lineEditAdditionalText->setText(additional.attribute("text"));
    int curPayment = s.value("payments").toString().split("|").indexOf(additional.attribute("paymentType"));

    if (curPayment == s.value("payments").toString().split("|").count() - 1)
    {
        disconnect(comboBoxPayment, SIGNAL(currentIndexChanged (QString)), this, SLOT(payTextChanged(QString)));

        comboBoxPayment->setCurrentIndex(curPayment);

        custPaymData = new CustomPaymData();
        custPaymData->payment1 = additional.attribute("payment1");
        custPaymData->amount1  = additional.attribute("amount1").toDouble();
        custPaymData->date1    = QDate::fromString(additional.attribute("liabDate1"), s.getDateFormat());
        custPaymData->payment2 = additional.attribute("payment2");
        custPaymData->amount2  = additional.attribute("amount2").toDouble();
        custPaymData->date2    = QDate::fromString(additional.attribute("liabDate2"), s.getDateFormat());

        connect(comboBoxPayment, SIGNAL(currentIndexChanged (QString)), this, SLOT(payTextChanged(QString)));
    } else {
        comboBoxPayment->setCurrentIndex(curPayment);
    }

    dateEditDayOfPayment->setDate(QDate::fromString(additional.attribute("liabDate"), s.getDateFormat()));
    int curCurrency = s.value(s.keyName(s.CURRENCIES)).toString().split("|").indexOf(additional.attribute("currency"));
    comboBoxCurrency->setCurrentIndex(curCurrency);

    int corrReason = s.value(s.keyName(s.CORRECTION_REASON)).toString().split("|").indexOf(additional.attribute("reason"));
    reasonCombo->setCurrentIndex(corrReason);

    unsaved = false;
    pushButtonSave->setEnabled(false);

    setIsEditAllowed(s.value(s.keyName(s.EDIT)).toBool());
    calculateDiscount();
    calculateSum();
}

/** Sets the controls into disabled/enabled state
 */
void CorrectiveInvoiceDialog::setIsEditAllowed(bool isAllowed)
{
//    if (editMode == false)
//    {
//        isAllowed = true;
//        unsaved = true;
//    }

    isEdit = true;
    lineEditInvNumber->setEnabled(isAllowed);
    dateEditDateOfSell->setEnabled(isAllowed);
    dateEditDateOfIssuance->setEnabled(isAllowed);
    //tableWidgetCommodities->setEnabled(isAllowed);
    spinBoxDiscount->setEnabled(false); // don't allow for now
    labelDiscount1->setEnabled(false); // don't allow for now
    comboBoxPayment->setEnabled(isAllowed);
    dateEditDayOfPayment->setEnabled(isAllowed);
    lineEditAdditionalText->setEnabled(isAllowed);
    pushButtonAddCommodity->setEnabled(isAllowed);
    pushButtonRemoveCommodity->setEnabled(isAllowed);
    pushButtonEditCommodity->setEnabled(isAllowed);
    checkBoxDiscount->setEnabled(false); // don't allow for now
    comboBoxCounterparties->setEnabled(false); // don't allow to change kontrahent
    comboBoxCurrency->setEnabled(isAllowed);
    pushButtonSave->setEnabled(isAllowed);
    dateEditDayOfPayment->setEnabled(isAllowed);

    if (!isAllowed && comboBoxPayment->currentIndex() > 0) {
        dateEditDayOfPayment->setEnabled(true);
    } else {
        dateEditDayOfPayment->setEnabled(false);
    }
    reasonCombo->setEnabled(isAllowed);

    //@TODO move it somewhere else, needs to be called after readData
    createOriginalInv();
}

/**  Creates object with the orignal invoice
 */
void CorrectiveInvoiceDialog::createOriginalInv()
{
    //invData.counterpartyID = lineEditCounterpartyName->text();

//    for (int i = 0; i < tableWidgetCommodities->rowCount(); ++i)
//    {
//        CommodityData commodity;
//        commodity.id = tableWidgetCommodities->item(i, 0)->text().toInt();
//        commodity.name = tableWidgetCommodities->item(i, 1)->text();
//        //commodity.postalCode = tableWidgetCommodities->item(i, 2)->text();
//        commodity.pkwiu = tableWidgetCommodities->item(i, 3)->text();
//        commodity.quantity = tableWidgetCommodities->item(i, 4)->text().toDouble();
//        //commodity.unit_id = tableWidgetCommodities->item(i, 5)->text();
//        commodity.discount = tableWidgetCommodities->item(i, 6)->text().toDouble();
//        commodity.net1 = tableWidgetCommodities->item(i, 8)->text().toDouble();
//        commodity.vat_id = tableWidgetCommodities->item(i, 9)->text().toInt();
//        invData.products[i] = commodity;
//    }

    invData.paymentDate = dateEditDayOfPayment->date();
    invData.sellingDate = dateEditDateOfSell->date();
    invData.issuanceDate = dateEditDateOfIssuance->date();
    invData.invNumber  = lineEditInvNumber->text();
    invData.paymentID = comboBoxPayment->currentIndex();
    invData.currencyID = comboBoxCurrency->currentIndex();
    invData.additText = lineEditAdditionalText->text();
}

/** Not used in this class
 */
void CorrectiveInvoiceDialog::calculateDiscount(){
}

/** Calculates the sums original invoice and the new one
 */
void CorrectiveInvoiceDialog::calculateSum(){
    double netto = 0, price = 0, quantity = 0, gross = 00;
    double discountValue = 0;
    netTotal = 0;
    discountTotal = 0;
    grossTotal = 0;
    origDiscTotal = 0;
    origNettTotal = 0;
    diffTotal = 0;

    // sum of after correction invoice
    SettingsGlobal s;
    for (int i = 0; i < tableWidgetCommodities->rowCount(); ++i)
    {
        price = s.stringToDouble(tableWidgetCommodities->item(i, 7)->text());
        quantity = s.stringToDouble(tableWidgetCommodities->item(i, 4)->text());
        netto = s.stringToDouble(tableWidgetCommodities->item(i, 8)->text());
        gross = s.stringToDouble(tableWidgetCommodities->item(i, 10)->text());
        discountValue = (price * quantity) - netto;
        netTotal += netto;
        discountTotal += discountValue;
        grossTotal += gross;
    }

    // initially origGrossTotal is -1
    // if it's -1 will set to to 0 and go through whole calculation
    if (origGrossTotal < 0) { //TODO lack of initialization
        origGrossTotal = 0;
        createOriginalInv();

        for (QMap<int, CommodityData>::const_iterator iter =
             invData.products.begin(); iter != invData.products.end(); ++iter)
        {
            //origGrossTotal += iter.value().gross;
            //origDiscTotal += iter.value().discount;
            //origNettTotal += iter.value().net1;
        }
    }

    diffTotal = grossTotal - origGrossTotal;
    labelSumNetVal->setText(s.numberToString(grossTotal, 'f', 2));
    labelDiscountVal->setText(s.numberToString(origGrossTotal, 'f', 2));
    labelSumGrossVal->setText(s.numberToString(diffTotal, 'f', 2));

    if (diffTotal < 0) {
        labelSumGross->setText(trUtf8("Do zwrotu:"));
    } else {
        labelSumGross->setText(trUtf8("Do zapłaty:"));
    }
}


void CorrectiveInvoiceDialog::calculateOneDiscount(const int i) {
    double quantity = 0, vat = 0, gross = 0;
    double netto = 0,  price = 0;
    double discountValue = 0, discount;

    SettingsGlobal s;
    price = s.stringToDouble(tableWidgetCommodities->item(i, 7)->text());
    if (checkBoxDiscount->isChecked()) {
        discount = spinBoxDiscount->value() * 0.01;
    } else {
        discount = s.stringToDouble(tableWidgetCommodities->item(i, 6)->text()) * 0.01;
    }
    quantity = s.stringToDouble(tableWidgetCommodities->item(i, 4)->text());
    netto = (price * quantity);
    discountValue = netto * discount;
    netto -= discountValue;
    vat = s.stringToDouble(tableWidgetCommodities->item(i, 9)->text());
    gross = netto * ((vat * 0.01) + 1);

    tableWidgetCommodities->item(i, 6)->setText(s.numberToString(discount * 100, 'f', 0)); // discount
    tableWidgetCommodities->item(i, 8)->setText(s.numberToString(netto)); // nett
    tableWidgetCommodities->item(i, 10)->setText(s.numberToString(gross)); // gross
}


//*************** HTML methods START  *** *****************************
void CorrectiveInvoiceDialog::makeInvoceProductsTitle(const short a) {
    invStrList += "<tr align=\"center\"><td>";

    if (a==1) {
        invStrList += trUtf8("Pozycje na fakturze po korekcie:");
    }
    if (a==0) {
        invStrList += trUtf8("Pozycje na fakturze przed korektą:");
    }
}

void CorrectiveInvoiceDialog::makeBeforeCorrProducts(){

    invStrList += "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\" width=\"100%\">";

    makeInvoiceProductsHeader();

    for (QMap<int, CommodityData>::const_iterator iter = invData.products.begin();
         iter != invData.products.end();
         ++iter)
    {
        // qDebug() << iter.value()->toString();
//        fraStrList += "<tr valign=\"middle\" align=\"center\" class=\"products\">";
//        // lp, nazwa, kod, pkwiu, ilosc, jm, rabat, cena jm., netto, vat, brutto
//        if (sett().value("faktury_pozycje/Lp").toBool())
//            fraStrList += "<td align=\"center\">" + sett().numberToString(iter.key() + 1) + "</td>";
//        if (sett().value("faktury_pozycje/Nazwa") .toBool())
//            fraStrList += "<td align=\"left\">" + iter.value()->getName() + "</td>";
//        if (sett().value("faktury_pozycje/Kod") .toBool())
//            fraStrList += "<td align=\"center\">" + iter.value()->getCode() + "</td>";
//        if (sett().value("faktury_pozycje/pkwiu") .toBool())
//            fraStrList += "<td align=\"center\">" + iter.value()->getPkwiu() + "</td>";
//        if (sett().value("faktury_pozycje/ilosc") .toBool())
//            fraStrList += "<td align=\"center\">" + sett().numberToString(iter.value()->getQuantity()) + "</td>";
//        if (sett().value("faktury_pozycje/jm") .toBool())
//            fraStrList += "<td align=\"center\">" + iter.value()->getQuantityType() + "</td>";
//        if (sett().value("faktury_pozycje/cenajedn") .toBool())
//            fraStrList += "<td align=\"center\">" + sett().numberToString(iter.value()->getPrice()) + "</td>";
//        double discountVal = iter.value()->getNett() * (iter.value()->getDiscount() * 0.01);
//        double nettMinusDisc = iter.value()->getNett() - discountVal;
//        if (sett().value("faktury_pozycje/wartnetto") .toBool())
//            fraStrList += "<td align=\"center\">" + sett().numberToString(iter.value()->getNett())
//                    + "</td>"; // netto
//        if (sett().value("faktury_pozycje/rabatperc") .toBool())
//            fraStrList += "<td align=\"center\">" + sett().numberToString(iter.value()->getDiscount())
//                    + "% </td>"; // rabat
//        if (sett().value("faktury_pozycje/rabatval") .toBool())
//            fraStrList += "<td align=\"center\">" + sett().numberToString(discountVal, 'f',  2)	+ " </td>";
//        if (sett().value("faktury_pozycje/nettoafter") .toBool())
//            fraStrList += "<td align=\"center\">" + sett().numberToString(nettMinusDisc, 'f', 2) + "</td>";
//        if (sett().value("faktury_pozycje/vatval") .toBool())
//            fraStrList += "<td align=\"center\">" + sett().numberToString(iter.value()->getVat())
//                    + "%</td>";
//        double vatPrice = iter.value()->getGross() - iter.value()->getNett(); // brutt-nett :)
//        if (sett().value("faktury_pozycje/vatprice") .toBool())
//            fraStrList += "<td align=\"center\">" + sett().numberToString(vatPrice, 'f', 2)
//                    + "</td>";
//        if (sett().value("faktury_pozycje/bruttoval") .toBool())
//            fraStrList += "<td align=\"center\">" + sett().numberToString(iter.value()->getGross()) + "</td>";
//        fraStrList += "</tr>";
    }

    invStrList += "</table>";
}

void CorrectiveInvoiceDialog::makeBeforeCorrSumm()
{
    SettingsGlobal s;
    invStrList += "<br><table width=\"100%\" border=\"0\" cellpadding=\"5\">";
    double vatPrice = origGrossTotal - origNettTotal;
    invStrList += "<tr class=\"productsSumHeader\" valign=\"middle\">";
    invStrList += "<td width=\"67%\" align=\"right\">&nbsp;</td>";
    invStrList += "<td width=\"11%\" align=\"center\">" + trUtf8("Wartość Netto") + "</td>"; // netto
    invStrList += "<td width=\"11%\" align=\"center\">" + trUtf8("Kwota VAT") + "</td>";// vat
    invStrList += "<td width=\"11%\" align=\"center\">" + trUtf8("Wartość Brutto") + "</td>"; // brutto
    invStrList += "</tr>";
    invStrList += "<tr class=\"productsSum\" valign=\"middle\">";
    invStrList += "<td align=\"right\">" + trUtf8("Razem:") + "</td>";
    invStrList += "<td align=\"center\">" + s.numberToString(origNettTotal, 'f', 2) + "</td>"; // netto
    invStrList += "<td align=\"center\">" + s.numberToString(vatPrice, 'f', 2) + "</td>";// vat
    invStrList += "<td align=\"center\">" + s.numberToString(origGrossTotal, 'f', 2) + "</td>"; // brutto
    invStrList += "</tr>";
    invStrList += "</table>";

    invStrList += "<hr class=\"hrdiv1\">";
}

void CorrectiveInvoiceDialog::makeInvoiceSumm()
{
    SettingsGlobal s;
    invStrList += "<br><table width=\"100%\" border=\"0\" cellpadding=\"5\">";
    double vatPrice = grossTotal - netTotal;
    invStrList += "<tr class=\"productsSumHeader\" valign=\"middle\">";
    invStrList += "<td width=\"67%\" align=\"right\">&nbsp;</td>";
    invStrList += "<td width=\"11%\" align=\"center\">" + trUtf8("Wartość Netto") + "</td>"; // netto
    invStrList += "<td width=\"11%\" align=\"center\">" + trUtf8("Kwota VAT") + "</td>";// vat
    invStrList += "<td width=\"11%\" align=\"center\">" + trUtf8("Wartość Brutto") + "</td>"; // brutto
    invStrList += "</tr><tr class=\"productsSum\" valign=\"middle\">";
    invStrList += "<td align=\"right\">" + trUtf8("Razem:") + "</td>";
    invStrList += "<td align=\"center\">" + s.numberToString(netTotal, 'f', 2) + "</td>"; // netto
    invStrList += "<td align=\"center\">" + s.numberToString(vatPrice, 'f', 2) + "</td>";// vat
    invStrList += "<td align=\"center\">" + s.numberToString(grossTotal, 'f', 2) + "</td>"; // brutto
    invStrList += "</tr>";
    invStrList += "</table>";
}

void CorrectiveInvoiceDialog::makeInvoiceSummAll()
{
    SettingsGlobal s;
    //fraStrList += "<tr comment=\"podsumowanie\"><td>";
    invStrList += "<table width=\"100%\" border=\"0\" cellpadding=\"7\">";
    invStrList += "<tr class=\"summary\">";
    invStrList += "<td width=\"48%\">";

    invStrList += trUtf8("Wartość faktury: ") + s.numberToString(origGrossTotal) + "<br>";
    invStrList += trUtf8("Wartość korekty: ") + s.numberToString(grossTotal) + "<br>";
    if (diffTotal > 0) {
        invStrList += trUtf8("Do zapłaty: ");
        invStrList += labelSumGrossVal->text() + " " + comboBoxCurrency->currentText() + "<br>";
    } else if (diffTotal < 0) {
        invStrList += trUtf8("Do zwrotu: ");
        invStrList += labelSumGrossVal->text() + " " + comboBoxCurrency->currentText()+ "<br>";
    }
    invStrList += trUtf8("słownie:")
            + ConvertAmount::convertPL(labelSumGrossVal->text(), comboBoxCurrency->currentText())
            + "<br>";
    if (comboBoxPayment->currentIndex() == 0) {
        invStrList += trUtf8("forma płatności: ") + comboBoxPayment->currentText() + "<br><b>";
        invStrList += trUtf8("Zapłacono gotówką") + "<br>";
    } else if ((comboBoxPayment->currentIndex() == comboBoxPayment->count() -1) && (custPaymData != NULL)) {
        invStrList += "<span style=\"toPay\">";
        invStrList += QString(trUtf8("Zapłacono: ") + custPaymData->payment1 + ": "
                              +  s.numberToString(custPaymData->amount1) + " " + comboBoxCurrency->currentText() + " "
                              + custPaymData->date1.toString(s.getDateFormat()) + "<br>");
        invStrList += QString(trUtf8("Zaległości: ") + custPaymData->payment2 + ": "
                              +  s.numberToString(custPaymData->amount2) + " " + comboBoxCurrency->currentText() + " "
                              + custPaymData->date2.toString(s.getDateFormat()));
        invStrList += "</span>";
    }  else {
        invStrList += trUtf8("forma płatności: ") + comboBoxPayment->currentText() + "<br><b>";
        invStrList += "<span style=\"payDate\">";
        invStrList += trUtf8("termin płatności: ")
                + dateEditDayOfPayment->date().toString(s.getDateFormat())	+ "<br>";
        invStrList += "</span>";
    }

    invStrList += trUtf8("przyczyna korekty: ") +  reasonCombo->currentText() + "<br>";
    invStrList += "<span class=\"additionalText\">"	+ lineEditAdditionalText->text() + "</span>";

    invStrList += "</td>";
    invStrList += "<td width=\"4%\">&nbsp;</td>";
    invStrList += "<td width=\"48%\" valign=\"top\">";

    invStrList += "<table width=\"90%\" border=\"0\">";
    invStrList += "<tr><td colspan=\"4\"><span style=\"font-size:8pt; font-weight:600;\">";
    invStrList += trUtf8("Ogółem stawkami:");
    invStrList += "</span></td>"; // Ogółem stawkami:
    invStrList += "</tr>";
    invStrList += getGroupedSums();
    invStrList += "<tr>";
    invStrList += "<td>&nbsp;</td>"; // netto
    invStrList += "<td>&nbsp;</td>"; // stawka
    invStrList += "<td>&nbsp;</td>"; // podatek
    invStrList += "<td>&nbsp;</td>"; // brutto
    invStrList += "</tr>";
    invStrList += "</table>";

    invStrList += "</td></tr></table>";
}
//*************** HTML methods END  *** *****************************

