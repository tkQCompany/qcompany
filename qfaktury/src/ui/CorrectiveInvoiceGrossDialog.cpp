/*
 * KorektaBrutto.cpp
 *
 *  Created on: Apr 3, 2009
 *      Author: moux
 */

#include "CorrectiveInvoiceGrossDialog.h"
#include "ui_InvoiceDialog.h"

/* Constructor
 */
CorrectiveInvoiceGrossDialog::CorrectiveInvoiceGrossDialog(QWidget *parent, Database *db, const QModelIndex &idInvoice):
    QDialog(parent), ui_(new Ui::InvoiceDialog), db_(db)
{
    ui_->setupUi(this);
    init_();
}

CorrectiveInvoiceGrossDialog::~CorrectiveInvoiceGrossDialog()
{
    delete ui_;
}

void CorrectiveInvoiceGrossDialog::init_()
{
    setWindowTitle(InvoiceTypeData::name(InvoiceTypeData::CORRECTIVE_GROSS));
    ui_->comboBoxInvoiceType->setCurrentIndex(InvoiceTypeData::CORRECTIVE_GROSS - 1);
}


/** Caclulate Discount
 */
void CorrectiveInvoiceGrossDialog::calculateOneDiscount(const int i)
{
    double quantity = 0, vat = 0, gross = 0;
    double netto = 0,  price = 0;
    double discountValue = 0, discount;
    SettingsGlobal s;

    price = s.stringToDouble(ui_->tableWidgetCommodities->item(i, 7)->text());
    if(ui_->checkBoxDiscount->isChecked())
    {
        discount = ui_->spinBoxDiscount->value() * 0.01;
    }
    else
    {
        discount = s.stringToDouble(ui_->tableWidgetCommodities->item(i, 6)->text()) * 0.01;
    }
    quantity = s.stringToDouble(ui_->tableWidgetCommodities->item(i, 4)->text());
    price = price * quantity;
    discountValue = price * discount;

    gross = price - discountValue;
    int vatValue = s.stringToDouble(ui_->tableWidgetCommodities->item(i, 9)->text());
    vat = (gross * vatValue)/(100 + vatValue);

    netto = gross - vat;

    ui_->tableWidgetCommodities->item(i, 6)->setText(s.numberToString(discount * 100, 'f', 0)); // discount
    ui_->tableWidgetCommodities->item(i, 8)->setText(s.numberToString(netto)); // nett
    ui_->tableWidgetCommodities->item(i, 10)->setText(s.numberToString(gross)); // gross
}


/** Slot
 *  Add new towar
 */
void CorrectiveInvoiceGrossDialog::addCommodity()
{
    CommodityListGrossDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        const int rowNum = ui_->tableWidgetCommodities->rowCount() == 0 ? 0 : ui_->tableWidgetCommodities->rowCount() - 1;
        ui_->tableWidgetCommodities->insertRow(rowNum);
        for(int i = CommodityVisualFields::NAME; i <= CommodityVisualFields::DISCOUNT; ++i)
        {
            //ui_->tableWidgetCommodities->setItem(rowNum, i, new QTableWidgetItem(dialog.ret.field(i)));
        }
        ui_->pushButtonSave->setEnabled(true);
        //unsaved = true;
//        if (ui_->checkBoxDiscount->isChecked())
//            calculateDiscount();
        //calculateSum();
    }
}
