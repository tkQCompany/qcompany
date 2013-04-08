/*
 * FakrutaBrutto.cpp
 *
 *  Created on: Apr 2, 2009
 *      Author: TPIELECH
 */

#include "InvoiceGrossDialog.h"
#include "InvoiceDialogImpl.h"
#include "ui_InvoiceDialog.h"
#include "SettingsGlobal.h"

class InvoiceGrossDialog::InvoiceGrossDialogImpl: public InvoiceDialogImpl
{
public:
    InvoiceGrossDialogImpl(QWidget *parent, Database *database) :
        InvoiceDialogImpl(parent, database) {}
};


InvoiceGrossDialog::InvoiceGrossDialog(QWidget *parent, Database *db, const QModelIndex &idInvoice) :
    QDialog(parent), pImpl_(new InvoiceGrossDialogImpl(parent, db))
{
    pImpl_->ui->setupUi(this);
    pImpl_->init(InvoiceTypeData::GROSS, idInvoice);
}

InvoiceGrossDialog::~InvoiceGrossDialog()
{
    delete pImpl_;
}


/** Caclulate Discount
 */
void InvoiceGrossDialog::calculateOneDiscount(const int i)
{
	double quantity = 0, vat = 0, gross = 0;
	double netto = 0,  price = 0;
	double discountValue = 0, discount;

    SettingsGlobal s;

    price = s.stringToDouble(pImpl_->ui->tableWidgetCommodities->item(i, 7)->text());
    if(pImpl_->ui->checkBoxDiscount->isChecked()) {
        discount = pImpl_->ui->spinBoxDiscount->value() * 0.01;
    }
    else
    {
        discount = s.stringToDouble(pImpl_->ui->tableWidgetCommodities->item(i, 6)->text()) * 0.01;
	}
    quantity = s.stringToDouble(pImpl_->ui->tableWidgetCommodities->item(i, 4)->text());
    price = price * quantity;
    discountValue = price * discount;

    gross = price - discountValue;
    int vatValue = s.stringToDouble(pImpl_->ui->tableWidgetCommodities->item(i, 9)->text());
    vat = (gross * vatValue)/(100 + vatValue);

    netto = gross - vat;

    pImpl_->ui->tableWidgetCommodities->item(i, 6)->setText(s.numberToString(discount * 100, 'f', 0)); // discount
    pImpl_->ui->tableWidgetCommodities->item(i, 8)->setText(s.numberToString(netto)); // nett
    pImpl_->ui->tableWidgetCommodities->item(i, 10)->setText(s.numberToString(gross)); // gross
}

/** Slot
 *  Add new towar
 */
void InvoiceGrossDialog::addCommodity()
{
//    CommodityListGrossDialog dialog(this, db_);
//    if (dialog.exec() == QDialog::Accepted)
//    {
//        const int rowNum = ui_->tableWidgetCommodities->rowCount() == 0 ? 0 : ui_->tableWidgetCommodities->rowCount() - 1;
//        ui_->tableWidgetCommodities->insertRow(rowNum);

//        for(int i = CommodityVisualFields::NAME; i <= CommodityVisualFields::DISCOUNT; ++i)
//        {
//            ui_->tableWidgetCommodities->setItem(rowNum, i, new QTableWidgetItem(dialog.ret.field(i)));
//        }

//        ui_->pushButtonSave->setEnabled(true);
//        unsaved = true;
////        if (checkBoxDiscount->isChecked())
////            calculateDiscount();
//        calculateSum();
//	}
}
