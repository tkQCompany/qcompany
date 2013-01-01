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
    invType = InvoiceTypeData::BILL;
    setWindowTitle(InvoiceTypeData::InvoiceTypeToString(invType));
    comboBoxInvoiceType->setCurrentIndex(invType - 1);
}
