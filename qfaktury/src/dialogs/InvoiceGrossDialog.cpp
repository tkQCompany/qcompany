/*
 * FakrutaBrutto.cpp
 *
 *  Created on: Apr 2, 2009
 *      Author: TPIELECH
 */

#include "InvoiceGrossDialog.h"

// constructor
InvoiceGrossDialog::InvoiceGrossDialog(QWidget *parent, Database *db) :
    InvoiceDialog(parent, db)
{
    setWindowTitle(trUtf8("Faktura VAT Brutto"));
}


QString InvoiceGrossDialog::getInvoiceTypeAndSaveNr() {
    const QString ftype("FBrutto");
    sett().setValue("fvat", lineEditInvNumber->text());
	return ftype;
}

/** Caclulate Discount
 */
void InvoiceGrossDialog::calculateOneDiscount(const int i)
{
	double quantity = 0, vat = 0, gross = 0;
	double netto = 0,  price = 0;
	double discountValue = 0, discount;

    price = sett().stringToDouble(tableWidgetCommodities->item(i, 7)->text());
    if (checkBoxDiscount->isChecked()) {
        discount = spinBoxDiscount->value() * 0.01;
    }
    else
    {
        discount = sett().stringToDouble(tableWidgetCommodities->item(i, 6)->text()) * 0.01;
	}
    quantity = sett().stringToDouble(tableWidgetCommodities->item(i, 4)->text());
    price = price * quantity;
    discountValue = price * discount;

    gross = price - discountValue;
    int vatValue = sett().stringToDouble(tableWidgetCommodities->item(i, 9)->text());
    vat = (gross * vatValue)/(100 + vatValue);

    netto = gross - vat;

    tableWidgetCommodities->item(i, 6)->setText(sett().numberToString(discount * 100, 'f', 0)); // discount
    tableWidgetCommodities->item(i, 8)->setText(sett().numberToString(netto)); // nett
    tableWidgetCommodities->item(i, 10)->setText(sett().numberToString(gross)); // gross

}

/** Slot
 *  Add new towar
 */
void InvoiceGrossDialog::addCommodity()
{
    CommodityListGrossDialog dialog(this, db_);
    if (dialog.exec() == QDialog::Accepted)
    {
        const int rowNum = tableWidgetCommodities->rowCount() == 0 ? 0 : tableWidgetCommodities->rowCount() - 1;
        tableWidgetCommodities->insertRow(rowNum);

        for(int i = CommodityVisualFields::NAME; i <= CommodityVisualFields::DISCOUNT; ++i)
        {
            tableWidgetCommodities->setItem(rowNum, i, new QTableWidgetItem(dialog.ret.field(i)));
        }

        pushButtonSave->setEnabled(true);
        unsaved = true;
//        if (checkBoxDiscount->isChecked())
//            calculateDiscount();
        calculateSum();
	}
}
