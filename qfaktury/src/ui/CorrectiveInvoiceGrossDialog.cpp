/*
 * KorektaBrutto.cpp
 *
 *  Created on: Apr 3, 2009
 *      Author: moux
 */

#include "CorrectiveInvoiceGrossDialog.h"
#include "ui_InvoiceDialog.h"
#include "InvoiceDialogImpl.h"


class CorrectiveInvoiceGrossDialog::CorrectiveInvoiceGrossDialogImpl: public InvoiceDialogImpl
{
public:
    CorrectiveInvoiceGrossDialogImpl(QWidget *parent, Database *database) :
        InvoiceDialogImpl(parent, database) {}
};


CorrectiveInvoiceGrossDialog::CorrectiveInvoiceGrossDialog(QWidget *parent, Database *db, const QModelIndex &idInvoice):
    InvoiceDialog(parent, db, InvoiceTypeData::CORRECTIVE_GROSS, idInvoice, false),
    pImpl_(new CorrectiveInvoiceGrossDialogImpl(parent, db))
{
    setPImpl(pImpl_);
}

CorrectiveInvoiceGrossDialog::~CorrectiveInvoiceGrossDialog()
{
    delete pImpl_;
}

void CorrectiveInvoiceGrossDialog::init_()
{
    setWindowTitle(InvoiceTypeData::name(InvoiceTypeData::CORRECTIVE_GROSS));
    pImpl_->ui->comboBoxInvoiceType->setCurrentIndex(InvoiceTypeData::CORRECTIVE_GROSS - 1);
}


/** Caclulate Discount
 */
void CorrectiveInvoiceGrossDialog::calculateOneDiscount(const int i)
{
    double quantity = 0, vat = 0, gross = 0;
    double netto = 0,  price = 0;
    double discountValue = 0, discount;
    SettingsGlobal s;

    price = s.stringToDouble(pImpl_->ui->tableWidgetCommodities->item(i, 7)->text());
    if(pImpl_->ui->checkBoxDiscount->isChecked())
    {
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
void CorrectiveInvoiceGrossDialog::addCommodity()
{
    CommodityListGrossDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        const int rowNum = pImpl_->ui->tableWidgetCommodities->rowCount() == 0 ?
                    0 : pImpl_->ui->tableWidgetCommodities->rowCount() - 1;
        pImpl_->ui->tableWidgetCommodities->insertRow(rowNum);
        for(int i = CommodityVisualFields::NAME; i <= CommodityVisualFields::DISCOUNT; ++i)
        {
            //ui_->tableWidgetCommodities->setItem(rowNum, i, new QTableWidgetItem(dialog.ret.field(i)));
        }
        pImpl_->ui->pushButtonSave->setEnabled(true);
        //unsaved = true;
//        if (ui_->checkBoxDiscount->isChecked())
//            calculateDiscount();
        //calculateSum();
    }
}
