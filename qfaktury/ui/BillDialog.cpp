/*
 * Rachunek.cpp
 *
 *  Created on: Apr 2, 2009
 *      Author: moux
 */

#include "BillDialog.h"

// constructor
BillDialog::BillDialog(QWidget *parent, Database *db):
    InvoiceDialog(parent, db)
{
    init_();
}

void BillDialog::init_()
{
    setWindowTitle(InvoiceTypeData::name(InvoiceTypeData::BILL));
    comboBoxInvoiceType->setCurrentIndex(InvoiceTypeData::BILL - 1);
}
