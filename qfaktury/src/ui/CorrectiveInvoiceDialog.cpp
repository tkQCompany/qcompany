#include "CorrectiveInvoiceDialog.h"
#include "ui_InvoiceDialog.h"
#include "InvoiceDialogImpl.h"


class CorrectiveInvoiceDialog::CorrectiveInvoiceDialogImpl: public InvoiceDialogImpl
{
public:
    CorrectiveInvoiceDialogImpl(QWidget *parent, Database *database) :
        InvoiceDialogImpl(parent, database) {}
};


CorrectiveInvoiceDialog::CorrectiveInvoiceDialog(QWidget *parent, Database *db, InvoiceTypeData::Type invoiceType, const QModelIndex &idInvoice):
    QDialog(parent), pImpl_(new CorrectiveInvoiceDialogImpl(parent, db))
{
    pImpl_->ui->setupUi(this);
    pImpl_->init(invoiceType, idInvoice);
    init_();
}

CorrectiveInvoiceDialog::~CorrectiveInvoiceDialog()
{
    delete pImpl_;
}


void CorrectiveInvoiceDialog::init_()
{
    SettingsGlobal s;
    pImpl_->ui->comboBoxReasonOfCorrection->addItems(s.value(s.CORRECTION_REASONS)
                                              .toString().split("|"));
    pImpl_->ui->comboBoxReasonOfCorrection->setEnabled(true);
    pImpl_->ui->labelReasonOfCorrection->setEnabled(true);

    pImpl_->ui->labelSumNet->setText(trUtf8("Wartość korekty:"));
    pImpl_->ui->labelDiscount2->setText(trUtf8("Wartość faktury:"));
    pImpl_->ui->labelSumGross->setText(trUtf8("Do zapłaty:"));

    connect(pImpl_->ui->comboBoxReasonOfCorrection, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(textChanged(QString)));
}



/** Slot
 *  Generate invoice and show preview
 */
//void CorrectiveInvoiceDialog::printInvoice()
//{
//    invStrList.clear();

//    makeInvoiceHeaderHTML(comboBoxInvoiceType->currentIndex() + 1);

//    makeInvoiceHeader(comboBoxInvoiceType->currentIndex() + 1, false, false, true);
//    makeInvoiceBody();
//    makeInvoceProductsTitle(0);
//    makeBeforeCorrProducts();
//    makeBeforeCorrSumm();
//    makeInvoceProductsTitle(1);
//    makeInvoiceProducts();
//    makeInvoiceSumm();
//    makeInvoiceSummAll();
//    makeInvoiceFooter();

//    SettingsGlobal s;
//    const int numberOfCopies = s.value(s.NUMBER_OF_COPIES), 2).toInt();
//    for (int i = 1; i <= numberOfCopies; i++)
//    {
//        makeInvoiceHeader(comboBoxInvoiceType->currentIndex() + 1, false, true, false);
//        makeInvoiceBody();
//        makeInvoceProductsTitle(0);
//        makeBeforeCorrProducts();
//        makeBeforeCorrSumm();
//        makeInvoceProductsTitle(1);
//        makeInvoiceProducts();
//        makeInvoiceSumm();
//        makeInvoiceSummAll();
//        makeInvoiceFooter();
//    }

//    makeInvoiceFooterHtml();

//    //print();
//}

/** Slot
 *  Validate close and save if requested
 */
void CorrectiveInvoiceDialog::canQuit()
{
//    if (!unsaved)
//    {
//        accept();
//    }
//    else
    {
        if (QMessageBox::warning(this, qApp->applicationName(), trUtf8("Dane zostały zmienione, czy chcesz zapisać?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            reject();
        }
        else
        {
            //saveInvoice();
//            if (saveFailed)
//            {
//                return;
//            }
//            accept();
        }
    }
}
//*********************************************** SLOTS END ****************************************/



/** Sets the controls into disabled/enabled state
 */
void CorrectiveInvoiceDialog::setIsEditAllowed(bool isAllowed)
{
//    if (editMode == false)
//    {
//        isAllowed = true;
//        unsaved = true;
//    }

    //isEdit = true;
    pImpl_->ui->lineEditInvNumber->setEnabled(isAllowed);
    pImpl_->ui->dateEditDateOfSell->setEnabled(isAllowed);
    pImpl_->ui->dateEditDateOfIssuance->setEnabled(isAllowed);
    //tableWidgetCommodities->setEnabled(isAllowed);
    pImpl_->ui->spinBoxDiscount->setEnabled(false); // don't allow for now
    pImpl_->ui->labelDiscount1->setEnabled(false); // don't allow for now
    pImpl_->ui->comboBoxPayment->setEnabled(isAllowed);
    pImpl_->ui->dateEditDayOfPayment->setEnabled(isAllowed);
    pImpl_->ui->lineEditAdditionalText->setEnabled(isAllowed);
    pImpl_->ui->pushButtonAddCommodity->setEnabled(isAllowed);
    pImpl_->ui->pushButtonRemoveCommodity->setEnabled(isAllowed);
    pImpl_->ui->pushButtonEditCommodity->setEnabled(isAllowed);
    pImpl_->ui->checkBoxDiscount->setEnabled(false); // don't allow for now
    pImpl_->ui->comboBoxCounterparties->setEnabled(false); // don't allow to change kontrahent
    pImpl_->ui->comboBoxCurrency->setEnabled(isAllowed);
    pImpl_->ui->pushButtonSave->setEnabled(isAllowed);
    pImpl_->ui->dateEditDayOfPayment->setEnabled(isAllowed);

    if (!isAllowed && pImpl_->ui->comboBoxPayment->currentIndex() > 0) {
        pImpl_->ui->dateEditDayOfPayment->setEnabled(true);
    } else {
        pImpl_->ui->dateEditDayOfPayment->setEnabled(false);
    }
    pImpl_->ui->comboBoxReasonOfCorrection->setEnabled(isAllowed);

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

//    invData.paymentDate_ = dateEditDayOfPayment->date();
//    invData.sellingDate_ = dateEditDateOfSell->date();
//    invData.issuanceDate_ = dateEditDateOfIssuance->date();
//    invData.invNumber_  = lineEditInvNumber->text();
//    invData.paymentID_ = comboBoxPayment->currentIndex();
//    invData.currencyID_ = comboBoxCurrency->currentIndex();
//    invData.additText_ = lineEditAdditionalText->text();
}

/** Not used in this class
 */
void CorrectiveInvoiceDialog::calculateDiscount(){
}

/** Calculates the sums original invoice and the new one
 */
void CorrectiveInvoiceDialog::calculateSum(){
//    double netto = 0, price = 0, quantity = 0, gross = 00;
//    double discountValue = 0;
//    netTotal = 0;
//    discountTotal = 0;
//    grossTotal = 0;
//    origDiscTotal = 0;
//    origNettTotal = 0;
//    diffTotal = 0;

//    // sum of after correction invoice
//    SettingsGlobal s;
//    for (int i = 0; i < tableWidgetCommodities->rowCount(); ++i)
//    {
//        price = s.stringToDouble(tableWidgetCommodities->item(i, 7)->text());
//        quantity = s.stringToDouble(tableWidgetCommodities->item(i, 4)->text());
//        netto = s.stringToDouble(tableWidgetCommodities->item(i, 8)->text());
//        gross = s.stringToDouble(tableWidgetCommodities->item(i, 10)->text());
//        discountValue = (price * quantity) - netto;
//        netTotal += netto;
//        discountTotal += discountValue;
//        grossTotal += gross;
//    }

//    // initially origGrossTotal is -1
//    // if it's -1 will set to to 0 and go through whole calculation
//    if (origGrossTotal < 0) { //TODO lack of initialization
//        origGrossTotal = 0;
//        createOriginalInv();

////        for (QMap<int, CommodityData>::const_iterator iter =
////             invData.products.begin(); iter != invData.products.end(); ++iter)
//        {
//            //origGrossTotal += iter.value().gross;
//            //origDiscTotal += iter.value().discount;
//            //origNettTotal += iter.value().net1;
//        }
//    }

//    diffTotal = grossTotal - origGrossTotal;
//    labelSumNetVal->setText(s.numberToString(grossTotal, 'f', 2));
//    labelDiscountVal->setText(s.numberToString(origGrossTotal, 'f', 2));
//    labelSumGrossVal->setText(s.numberToString(diffTotal, 'f', 2));

//    if (diffTotal < 0) {
//        labelSumGross->setText(trUtf8("Do zwrotu:"));
//    } else {
//        labelSumGross->setText(trUtf8("Do zapłaty:"));
//    }
}


void CorrectiveInvoiceDialog::calculateOneDiscount(const int) {
//    double quantity = 0, vat = 0, gross = 0;
//    double netto = 0,  price = 0;
//    double discountValue = 0, discount;

//    SettingsGlobal s;
//    price = s.stringToDouble(ui_->tableWidgetCommodities->item(i, 7)->text());
//    if (checkBoxDiscount->isChecked()) {
//        discount = spinBoxDiscount->value() * 0.01;
//    } else {
//        discount = s.stringToDouble(tableWidgetCommodities->item(i, 6)->text()) * 0.01;
//    }
//    quantity = s.stringToDouble(tableWidgetCommodities->item(i, 4)->text());
//    netto = (price * quantity);
//    discountValue = netto * discount;
//    netto -= discountValue;
//    vat = s.stringToDouble(tableWidgetCommodities->item(i, 9)->text());
//    gross = netto * ((vat * 0.01) + 1);

//    tableWidgetCommodities->item(i, 6)->setText(s.numberToString(discount * 100, 'f', 0)); // discount
//    tableWidgetCommodities->item(i, 8)->setText(s.numberToString(netto)); // nett
//    tableWidgetCommodities->item(i, 10)->setText(s.numberToString(gross)); // gross
}


//*************** HTML methods START  *** *****************************
//void CorrectiveInvoiceDialog::makeInvoceProductsTitle(const short a) {
//    invStrList += "<tr align=\"center\"><td>";

//    if (a==1) {
//        invStrList += trUtf8("Pozycje na fakturze po korekcie:");
//    }
//    if (a==0) {
//        invStrList += trUtf8("Pozycje na fakturze przed korektą:");
//    }
//}

//void CorrectiveInvoiceDialog::makeBeforeCorrProducts(){

//    invStrList += "<table border=\"1\" cellspacing=\"0\" cellpadding=\"5\" width=\"100%\">";

//    makeInvoiceProductsHeader();

//    for (QMap<int, CommodityData>::const_iterator iter = invData.products.begin();
//         iter != invData.products.end();
//         ++iter)
//    {
//        // qDebug() << iter.value()->toString();
////        fraStrList += "<tr valign=\"middle\" align=\"center\" class=\"products\">";
////        // lp, nazwa, kod, pkwiu, ilosc, jm, rabat, cena jm., netto, vat, brutto
////        if (sett().value("faktury_pozycje/Lp").toBool())
////            fraStrList += "<td align=\"center\">" + sett().numberToString(iter.key() + 1) + "</td>";
////        if (sett().value("faktury_pozycje/Nazwa") .toBool())
////            fraStrList += "<td align=\"left\">" + iter.value()->getName() + "</td>";
////        if (sett().value("faktury_pozycje/Kod") .toBool())
////            fraStrList += "<td align=\"center\">" + iter.value()->getCode() + "</td>";
////        if (sett().value("faktury_pozycje/pkwiu") .toBool())
////            fraStrList += "<td align=\"center\">" + iter.value()->getPkwiu() + "</td>";
////        if (sett().value("faktury_pozycje/ilosc") .toBool())
////            fraStrList += "<td align=\"center\">" + sett().numberToString(iter.value()->getQuantity()) + "</td>";
////        if (sett().value("faktury_pozycje/jm") .toBool())
////            fraStrList += "<td align=\"center\">" + iter.value()->getQuantityType() + "</td>";
////        if (sett().value("faktury_pozycje/cenajedn") .toBool())
////            fraStrList += "<td align=\"center\">" + sett().numberToString(iter.value()->getPrice()) + "</td>";
////        double discountVal = iter.value()->getNett() * (iter.value()->getDiscount() * 0.01);
////        double nettMinusDisc = iter.value()->getNett() - discountVal;
////        if (sett().value("faktury_pozycje/wartnetto") .toBool())
////            fraStrList += "<td align=\"center\">" + sett().numberToString(iter.value()->getNett())
////                    + "</td>"; // netto
////        if (sett().value("faktury_pozycje/rabatperc") .toBool())
////            fraStrList += "<td align=\"center\">" + sett().numberToString(iter.value()->getDiscount())
////                    + "% </td>"; // rabat
////        if (sett().value("faktury_pozycje/rabatval") .toBool())
////            fraStrList += "<td align=\"center\">" + sett().numberToString(discountVal, 'f',  2)	+ " </td>";
////        if (sett().value("faktury_pozycje/nettoafter") .toBool())
////            fraStrList += "<td align=\"center\">" + sett().numberToString(nettMinusDisc, 'f', 2) + "</td>";
////        if (sett().value("faktury_pozycje/vatval") .toBool())
////            fraStrList += "<td align=\"center\">" + sett().numberToString(iter.value()->getVat())
////                    + "%</td>";
////        double vatPrice = iter.value()->getGross() - iter.value()->getNett(); // brutt-nett :)
////        if (sett().value("faktury_pozycje/vatprice") .toBool())
////            fraStrList += "<td align=\"center\">" + sett().numberToString(vatPrice, 'f', 2)
////                    + "</td>";
////        if (sett().value("faktury_pozycje/bruttoval") .toBool())
////            fraStrList += "<td align=\"center\">" + sett().numberToString(iter.value()->getGross()) + "</td>";
////        fraStrList += "</tr>";
//    }

//    invStrList += "</table>";
//}

//void CorrectiveInvoiceDialog::makeBeforeCorrSumm()
//{
//    SettingsGlobal s;
//    invStrList += "<br><table width=\"100%\" border=\"0\" cellpadding=\"5\">";
//    double vatPrice = origGrossTotal - origNettTotal;
//    invStrList += "<tr class=\"productsSumHeader\" valign=\"middle\">";
//    invStrList += "<td width=\"67%\" align=\"right\">&nbsp;</td>";
//    invStrList += "<td width=\"11%\" align=\"center\">" + trUtf8("Wartość Netto") + "</td>"; // netto
//    invStrList += "<td width=\"11%\" align=\"center\">" + trUtf8("Kwota VAT") + "</td>";// vat
//    invStrList += "<td width=\"11%\" align=\"center\">" + trUtf8("Wartość Brutto") + "</td>"; // brutto
//    invStrList += "</tr>";
//    invStrList += "<tr class=\"productsSum\" valign=\"middle\">";
//    invStrList += "<td align=\"right\">" + trUtf8("Razem:") + "</td>";
//    invStrList += "<td align=\"center\">" + s.numberToString(origNettTotal, 'f', 2) + "</td>"; // netto
//    invStrList += "<td align=\"center\">" + s.numberToString(vatPrice, 'f', 2) + "</td>";// vat
//    invStrList += "<td align=\"center\">" + s.numberToString(origGrossTotal, 'f', 2) + "</td>"; // brutto
//    invStrList += "</tr>";
//    invStrList += "</table>";

//    invStrList += "<hr class=\"hrdiv1\">";
//}

//void CorrectiveInvoiceDialog::makeInvoiceSumm()
//{
//    SettingsGlobal s;
//    invStrList += "<br><table width=\"100%\" border=\"0\" cellpadding=\"5\">";
//    double vatPrice = grossTotal - netTotal;
//    invStrList += "<tr class=\"productsSumHeader\" valign=\"middle\">";
//    invStrList += "<td width=\"67%\" align=\"right\">&nbsp;</td>";
//    invStrList += "<td width=\"11%\" align=\"center\">" + trUtf8("Wartość Netto") + "</td>"; // netto
//    invStrList += "<td width=\"11%\" align=\"center\">" + trUtf8("Kwota VAT") + "</td>";// vat
//    invStrList += "<td width=\"11%\" align=\"center\">" + trUtf8("Wartość Brutto") + "</td>"; // brutto
//    invStrList += "</tr><tr class=\"productsSum\" valign=\"middle\">";
//    invStrList += "<td align=\"right\">" + trUtf8("Razem:") + "</td>";
//    invStrList += "<td align=\"center\">" + s.numberToString(netTotal, 'f', 2) + "</td>"; // netto
//    invStrList += "<td align=\"center\">" + s.numberToString(vatPrice, 'f', 2) + "</td>";// vat
//    invStrList += "<td align=\"center\">" + s.numberToString(grossTotal, 'f', 2) + "</td>"; // brutto
//    invStrList += "</tr>";
//    invStrList += "</table>";
//}

//void CorrectiveInvoiceDialog::makeInvoiceSummAll()
//{
//    SettingsGlobal s;
//    //fraStrList += "<tr comment=\"podsumowanie\"><td>";
//    invStrList += "<table width=\"100%\" border=\"0\" cellpadding=\"7\">";
//    invStrList += "<tr class=\"summary\">";
//    invStrList += "<td width=\"48%\">";

//    invStrList += trUtf8("Wartość faktury: ") + s.numberToString(origGrossTotal) + "<br>";
//    invStrList += trUtf8("Wartość korekty: ") + s.numberToString(grossTotal) + "<br>";
//    if (diffTotal > 0) {
//        invStrList += trUtf8("Do zapłaty: ");
//        invStrList += labelSumGrossVal->text() + " " + comboBoxCurrency->currentText() + "<br>";
//    } else if (diffTotal < 0) {
//        invStrList += trUtf8("Do zwrotu: ");
//        invStrList += labelSumGrossVal->text() + " " + comboBoxCurrency->currentText()+ "<br>";
//    }
//    invStrList += trUtf8("słownie:")
//            + ConvertAmount::convertPL(labelSumGrossVal->text(), comboBoxCurrency->currentText())
//            + "<br>";
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

//    invStrList += trUtf8("przyczyna korekty: ") +  reasonCombo->currentText() + "<br>";
//    invStrList += "<span class=\"additionalText\">"	+ lineEditAdditionalText->text() + "</span>";

//    invStrList += "</td>";
//    invStrList += "<td width=\"4%\">&nbsp;</td>";
//    invStrList += "<td width=\"48%\" valign=\"top\">";

//    invStrList += "<table width=\"90%\" border=\"0\">";
//    invStrList += "<tr><td colspan=\"4\"><span style=\"font-size:8pt; font-weight:600;\">";
//    invStrList += trUtf8("Ogółem stawkami:");
//    invStrList += "</span></td>"; // Ogółem stawkami:
//    invStrList += "</tr>";
//    invStrList += getGroupedSums();
//    invStrList += "<tr>";
//    invStrList += "<td>&nbsp;</td>"; // netto
//    invStrList += "<td>&nbsp;</td>"; // stawka
//    invStrList += "<td>&nbsp;</td>"; // podatek
//    invStrList += "<td>&nbsp;</td>"; // brutto
//    invStrList += "</tr>";
//    invStrList += "</table>";

//    invStrList += "</td></tr></table>";
//}
//*************** HTML methods END  *** *****************************
