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
}

void BillDialog::init()
{
    invType = InvoiceTypeData::BILL;
    setWindowTitle(InvoiceTypeData::InvoiceTypeToString(invType));
}
